/*=============================================================================*/
// Copyright 2022-2023 Smile Engine
// Authors: Zenn Geeraerts
/*=============================================================================*/
#include "smpch.h"
#include "script_engine.h"

#include "script_glue.h"

#include <fstream>

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

namespace smile::scripting
{
    namespace utils
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

                SM_LOG_TRACE( "%s.%s\n", nameSpace, name );
            }
        }
    }

    struct ScriptEngineData final
    {
        MonoDomain *pRootDomain = nullptr;
        MonoDomain *pAppDomain = nullptr;

        MonoAssembly *pCoreAssembly = nullptr;
        MonoImage *pCoreAssemblyImage = nullptr;

        MonoAssembly *pAppAssembly = nullptr;
        MonoImage *pAppAssemblyImage = nullptr;

        ScriptClass EntityClass;

        std::unordered_map< std::string, Ref< ScriptClass > > EntityClasses;
        std::unordered_map< UUID, Ref< ScriptInstance > > EntityInstances;

        // Runtime
        scene::Scene *pSceneContext = nullptr;
    };

    static ScriptEngineData *s_pData = nullptr;

    void ScriptEngine::Initialize()
    {
        s_pData = new ScriptEngineData{};

        InitializeMono();

#ifdef SM_C_DEBUG
        LoadAssembly( "resources/scripts/Debug/smile-script-core.dll" );
        LoadAppAssembly( "sandbox-project/assets/scripts/bin/Debug/sandbox.dll" );
#elif SM_C_RELEASE
        LoadAssembly( "resources/scripts/Release/smile-script-core.dll" );
        LoadAppAssembly( "sandbox-project/assets/scripts/bin/Release/sandbox.dll" );
#endif

        LoadAssemblyClasses();

        ScriptGlue::RegisterComponentTypes();
        ScriptGlue::RegisterFunctions();

        // Retrieve and instantiate class
        s_pData->EntityClass = ScriptClass{ "Smile", "Entity", true };
#if 0
        MonoObject *pInstance = s_pData->EntityClass.Instantiate();

        MonoMethod *pPrintIntFunc = s_pData->EntityClass.GetMethod( "PrintInt", 1 );
        int value = 5;
        void *pParams[1] = { &value };
        s_pData->EntityClass.InvokeMethod( pInstance, pPrintIntFunc, pParams );

        MonoString *pMonoString = mono_string_new( s_pData->pAppDomain, "Hello world from C++" );
        void *pStringParam = pMonoString;
        MonoMethod *pPrintCustomMessageFunc = s_pData->EntityClass.GetMethod( "PrintCustomMessage", 1 );
        s_pData->EntityClass.InvokeMethod( pInstance, pPrintCustomMessageFunc, &pStringParam );
#endif
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
        // mono_domain_unload( data->appDomain );
        s_pData->pAppDomain = nullptr;

        // mono_jit_cleanup( data->rootDomain );
        s_pData->pRootDomain = nullptr;
    }

    void ScriptEngine::LoadAssembly( const std::filesystem::path &filePath )
    {
        s_pData->pAppDomain = mono_domain_create_appdomain( ( char * )"SmileScriptRuntime", nullptr );
        mono_domain_set( s_pData->pAppDomain, true );

        s_pData->pCoreAssembly = utils::LoadMonoAssembly( filePath );
        s_pData->pCoreAssemblyImage = mono_assembly_get_image( s_pData->pCoreAssembly );
        // utils::printAssemblyTypes( data->coreAssembly );
    }

    void ScriptEngine::LoadAppAssembly( const std::filesystem::path &filePath )
    {
        s_pData->pAppAssembly = utils::LoadMonoAssembly( filePath );
        s_pData->pAppAssemblyImage = mono_assembly_get_image( s_pData->pAppAssembly );
    }

    void ScriptEngine::LoadAssemblyClasses()
    {
        s_pData->EntityClasses.clear();

        const MonoTableInfo *pTypeDefinitionsTable =
            mono_image_get_table_info( s_pData->pAppAssemblyImage, MONO_TABLE_TYPEDEF );
        Int32 typeCount = mono_table_info_get_rows( pTypeDefinitionsTable );
        MonoClass *pEntityClass = mono_class_from_name( s_pData->pCoreAssemblyImage, "Smile", "Entity" );

        for ( Int32 i = 0; i < typeCount; i++ )
        {
            Uint32 cols[MONO_TYPEDEF_SIZE];
            mono_metadata_decode_row( pTypeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE );

            const char *nameSpace =
                mono_metadata_string_heap( s_pData->pAppAssemblyImage, cols[MONO_TYPEDEF_NAMESPACE] );
            const char *name = mono_metadata_string_heap( s_pData->pAppAssemblyImage, cols[MONO_TYPEDEF_NAME] );
            std::string fullName;
            if ( strlen( nameSpace ) != 0 )
            {
                fullName = nameSpace;
                fullName.append( "." );
                fullName.append( name );
            }
            else
            {
                fullName = name;
            }

            MonoClass *pMonoClass = mono_class_from_name( s_pData->pAppAssemblyImage, nameSpace, name );

            if ( pMonoClass == pEntityClass )
                continue;

            bool isEntity = mono_class_is_subclass_of( pMonoClass, pEntityClass, false );
            if ( isEntity )
            {
                s_pData->EntityClasses[fullName] = CreateRef< ScriptClass >( nameSpace, name );
            }

            SM_LOG_TRACE( "%s.%s\n", nameSpace, name );
        }
    }

    MonoImage *ScriptEngine::GetCoreAssemblyImage()
    {
        return s_pData->pCoreAssemblyImage;
    }

    MonoObject *ScriptEngine::InstantiateClass( MonoClass *pMonoClass )
    {
        MonoObject *pInstance = mono_object_new( s_pData->pAppDomain, pMonoClass );
        mono_runtime_object_init( pInstance );
        return pInstance;
    }

    scene::Scene *ScriptEngine::GetSceneContext()
    {
        return s_pData->pSceneContext;
    }

    std::unordered_map< std::string, Ref< ScriptClass > > ScriptEngine::GetEntityClasses()
    {
        return s_pData->EntityClasses;
    }

    void ScriptEngine::OnRuntimeStart( scene::Scene *pScene )
    {
        s_pData->pSceneContext = pScene;
    }

    void ScriptEngine::OnRuntimeStop()
    {
        s_pData->pSceneContext = nullptr;
        s_pData->EntityInstances.clear();
    }

    bool ScriptEngine::EntityClassExists( const std::string &fullClassName )
    {
        return s_pData->EntityClasses.find( fullClassName ) != s_pData->EntityClasses.end();
    }

    void ScriptEngine::OnCreateEntity( scene::Entity entity )
    {
        const auto &scriptComponent = entity.GetComponent< scene::ScriptComponent >();
        if ( EntityClassExists( scriptComponent.ClassName ) )
        {
            Ref< ScriptInstance > pInstance =
                CreateRef< ScriptInstance >( s_pData->EntityClasses[scriptComponent.ClassName], entity );
            s_pData->EntityInstances[entity.GetUUID()] = pInstance;
            pInstance->InvokeOnCreate();
        }
    }

    void ScriptEngine::OnUpdateEntity( scene::Entity entity, Timestep deltaTime )
    {
        UUID entityUUID = entity.GetUUID();
        SM_ASSERT( s_pData->EntityInstances.find( entityUUID ) != s_pData->EntityInstances.end(),
            "ScriptEngine::OnUpdateEntity > Entity instance not found" );

        Ref< ScriptInstance > pInstance = s_pData->EntityInstances[entityUUID];
        pInstance->InvokeOnUpdate( deltaTime );
    }

    ScriptClass::ScriptClass( const std::string &classNamespace, const std::string &className, bool isCore )
        : m_ClassNamespace{ classNamespace }, m_ClassName{ className }
    {
        m_pMonoClass = mono_class_from_name( isCore ? s_pData->pCoreAssemblyImage : s_pData->pAppAssemblyImage,
            classNamespace.c_str(),
            className.c_str() );
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

    ScriptInstance::ScriptInstance( Ref< ScriptClass > pScriptClass, scene::Entity entity )
        : m_pScriptClass{ pScriptClass }
    {
        m_pInstance = pScriptClass->Instantiate();
        m_pConstructor = s_pData->EntityClass.GetMethod( ".ctor", 1 );
        m_pOnCreateMethod = pScriptClass->GetMethod( "OnCreate", 0 );
        m_pOnUpdateMethod = pScriptClass->GetMethod( "OnUpdate", 1 );

        UUID entityID = entity.GetUUID();
        void *pParam = &entityID;
        pScriptClass->InvokeMethod( m_pInstance, m_pConstructor, &pParam );
    }

    void ScriptInstance::InvokeOnCreate()
    {
        if ( m_pOnCreateMethod )
            m_pScriptClass->InvokeMethod( m_pInstance, m_pOnCreateMethod );
    }

    void ScriptInstance::InvokeOnUpdate( float deltaTime )
    {
        if ( m_pOnUpdateMethod )
        {
            void *pParam = &deltaTime;
            m_pScriptClass->InvokeMethod( m_pInstance, m_pOnUpdateMethod, &pParam );
        }
    }
}