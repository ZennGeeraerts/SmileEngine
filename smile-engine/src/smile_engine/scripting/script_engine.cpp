#include "smpch.h"
#include "script_engine.h"

#include "script_glue.h"

#include <fstream>

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

namespace smile::scripting
{
    namespace Utils
    {
        // TODO: move to FileSystem class
        static char *ReadBytes( const std::filesystem::path &filePath, Uint32 *pOutSize )
        {
            std::ifstream stream{ filePath, std::ios::binary | std::ios::ate };

            if ( !stream )
            {
                // Failed to open the file
                return nullptr;
            }

            std::streampos end = stream.tellg();
            stream.seekg( 0, std::ios::beg );
            Uint64 size = end - stream.tellg();

            if ( size == 0 )
            {
                // File is empty
                return nullptr;
            }

            char *pBuffer = new char[size];
            stream.read( pBuffer, size );
            stream.close();

            *pOutSize = static_cast< Uint64 >( size );
            return pBuffer;
        }

        static MonoAssembly *LoadMonoAssembly( const std::filesystem::path &assemblyPath )
        {
            Uint32 fileSize = 0;
            char *pFileData = ReadBytes( assemblyPath, &fileSize );

            // NOTE: We can't use this image for anything other than loading the assembly because this image doesn't
            // have a reference to the assembly
            MonoImageOpenStatus status;
            MonoImage *image = mono_image_open_from_data_full( pFileData, fileSize, 1, &status, 0 );

            if ( status != MONO_IMAGE_OK )
            {
                const char *errorMessage = mono_image_strerror( status );
                // Log some error message using the errorMessage data
                return nullptr;
            }

            std::string assemblyPathStr = assemblyPath.string();
            MonoAssembly *pAssembly = mono_assembly_load_from_full( image, assemblyPathStr.c_str(), &status, 0 );
            mono_image_close( image );

            // Don't forget to free the file data
            delete[] pFileData;

            return pAssembly;
        }

        static void PrintAssemblyTypes( MonoAssembly *pAssembly )
        {
            MonoImage *pImage = mono_assembly_get_image( pAssembly );
            const MonoTableInfo *pTypeDefinitionsTable = mono_image_get_table_info( pImage, MONO_TABLE_TYPEDEF );
            Int32 typeCount = mono_table_info_get_rows( pTypeDefinitionsTable );

            for ( Int32 i = 0; i < typeCount; i++ )
            {
                Uint32 cols[MONO_TYPEDEF_SIZE];
                mono_metadata_decode_row( pTypeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE );

                const char *nameSpace = mono_metadata_string_heap( pImage, cols[MONO_TYPEDEF_NAMESPACE] );
                const char *name = mono_metadata_string_heap( pImage, cols[MONO_TYPEDEF_NAME] );

                printf( "%s.%s\n", nameSpace, name );
            }
        }
    }

    struct ScriptEngineData final
    {
        MonoDomain *pRootDomain = nullptr;
        MonoDomain *pAppDomain = nullptr;

        MonoAssembly *pCoreAssembly = nullptr;
        MonoImage *pCoreAssemblyImage = nullptr;

        ScriptClass EntityClass;
    };

    static ScriptEngineData *s_pData = nullptr;

    void ScriptEngine::Initialize()
    {
        s_pData = new ScriptEngineData{};

        InitializeMono();
        LoadAssembly( "resources/scripts/Smile-ScriptCore.dll" );

        ScriptGlue::RegisterFunctions();

        // Retrieve and instantiate class
        s_pData->EntityClass = ScriptClass{ "Smile", "Entity" };

        MonoObject *pInstance = s_pData->EntityClass.Instantiate();

        MonoMethod *pPrintIntFunc = s_pData->EntityClass.GetMethod( "PrintInt", 1 );
        int value = 5;
        void *pParams[1] = { &value };
        s_pData->EntityClass.InvokeMethod( pInstance, pPrintIntFunc, pParams );

        MonoString *pMonoString = mono_string_new( s_pData->pAppDomain, "Hello world from C++" );
        void *pStringParam = pMonoString;
        MonoMethod *pPrintCustomMessageFunc = s_pData->EntityClass.GetMethod( "PrintCustomMessage", 1 );
        s_pData->EntityClass.InvokeMethod( pInstance, pPrintCustomMessageFunc, &pStringParam );
    }

    void ScriptEngine::ShutDown()
    {
        ShutDownMono();
        delete s_pData;
    }

    void ScriptEngine::InitializeMono()
    {
        mono_set_assemblies_path( "mono/lib" );

        MonoDomain *pRootDomain = mono_jit_init( "SmileJITRuntime" );
        SM_ASSERT( pRootDomain, "ScriptEngine::initializeMono > Cannot initialize root domain" );
        
        // Store the root domain pointer
        s_pData->pRootDomain = pRootDomain;
    }

    void ScriptEngine::ShutDownMono()
    {
        //mono_domain_unload( data->appDomain );
        s_pData->pAppDomain = nullptr;

        //mono_jit_cleanup( data->rootDomain );
        s_pData->pRootDomain = nullptr;
    }

    void ScriptEngine::LoadAssembly( const std::filesystem::path &filePath )
    {
        s_pData->pAppDomain = mono_domain_create_appdomain( ( char * )"SmileScriptRuntime", nullptr );
        mono_domain_set( s_pData->pAppDomain, true );

        s_pData->pCoreAssembly = Utils::LoadMonoAssembly( filePath );
        s_pData->pCoreAssemblyImage = mono_assembly_get_image( s_pData->pCoreAssembly );
        // utils::printAssemblyTypes( data->coreAssembly );
    }

    MonoObject *ScriptEngine::InstantiateClass( MonoClass *pMonoClass )
    {
        MonoObject *pInstance = mono_object_new( s_pData->pAppDomain, pMonoClass );
        mono_runtime_object_init( pInstance );
        return pInstance;
    }

    ScriptClass::ScriptClass( const std::string &classNamespace, const std::string &className )
        : m_ClassNamespace{ classNamespace }, m_ClassName{ className }
    {
        m_pMonoClass = mono_class_from_name( s_pData->pCoreAssemblyImage, classNamespace.c_str(), className.c_str() );
    }

    MonoObject *ScriptClass::Instantiate()
    {
        return ScriptEngine::InstantiateClass( m_pMonoClass );
    }

    MonoMethod *ScriptClass::GetMethod( const std::string &name, int parameterCount )
    {
        return mono_class_get_method_from_name( m_pMonoClass, name.c_str(), parameterCount );
    }

    MonoObject *ScriptClass::InvokeMethod( MonoObject *pInstance, MonoMethod *pMethod, void **ppParams )
    {
        return mono_runtime_invoke( pMethod, pInstance, ppParams, nullptr );
    }
}