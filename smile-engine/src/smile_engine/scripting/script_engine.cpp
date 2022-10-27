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
        static char *readBytes( const std::filesystem::path &file_path, Uint32 *out_size )
        {
            std::ifstream stream{ file_path, std::ios::binary | std::ios::ate };

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

            char *buffer = new char[size];
            stream.read( buffer, size );
            stream.close();

            *out_size = static_cast< Uint64 >( size );
            return buffer;
        }

        static MonoAssembly *loadMonoAssembly( const std::filesystem::path &assembly_path )
        {
            Uint32 file_size = 0;
            char *file_data = readBytes( assembly_path, &file_size );

            // NOTE: We can't use this image for anything other than loading the assembly because this image doesn't
            // have a reference to the assembly
            MonoImageOpenStatus status;
            MonoImage *image = mono_image_open_from_data_full( file_data, file_size, 1, &status, 0 );

            if ( status != MONO_IMAGE_OK )
            {
                const char *error_message = mono_image_strerror( status );
                // Log some error message using the errorMessage data
                return nullptr;
            }

            std::string assembly_path_str = assembly_path.string();
            MonoAssembly *assembly = mono_assembly_load_from_full( image, assembly_path_str.c_str(), &status, 0 );
            mono_image_close( image );

            // Don't forget to free the file data
            delete[] file_data;

            return assembly;
        }

        static void printAssemblyTypes( MonoAssembly *assembly )
        {
            MonoImage *image = mono_assembly_get_image( assembly );
            const MonoTableInfo *type_definitions_table = mono_image_get_table_info( image, MONO_TABLE_TYPEDEF );
            Int32 type_count = mono_table_info_get_rows( type_definitions_table );

            for ( Int32 i = 0; i < type_count; i++ )
            {
                Uint32 cols[MONO_TYPEDEF_SIZE];
                mono_metadata_decode_row( type_definitions_table, i, cols, MONO_TYPEDEF_SIZE );

                const char *name_space = mono_metadata_string_heap( image, cols[MONO_TYPEDEF_NAMESPACE] );
                const char *name = mono_metadata_string_heap( image, cols[MONO_TYPEDEF_NAME] );

                printf( "%s.%s\n", name_space, name );
            }
        }
    }

    struct ScriptEngineData final
    {
        MonoDomain *rootDomain = nullptr;
        MonoDomain *appDomain = nullptr;

        MonoAssembly *coreAssembly = nullptr;
        MonoImage *coreAssemblyImage = nullptr;

        ScriptClass entityClass;
    };

    static ScriptEngineData *data = nullptr;

    void ScriptEngine::initialize()
    {
        data = new ScriptEngineData{};

        initializeMono();
        loadAssembly( "resources/scripts/Smile-ScriptCore.dll" );

        ScriptGlue::registerFunctions();

        // Retrieve and instantiate class
        data->entityClass = ScriptClass{ "Smile", "Entity" };

        MonoObject *instance = data->entityClass.instantiate();

        MonoMethod *print_int_func = data->entityClass.getMethod( "PrintInt", 1 );
        int value = 5;
        void *params[1] = { &value };
        data->entityClass.invokeMethod( instance, print_int_func, params );

        MonoString *mono_string = mono_string_new( data->appDomain, "Hello world from C++" );
        void *string_param = mono_string;
        MonoMethod *print_custom_message_func = data->entityClass.getMethod( "PrintCustomMessage", 1 );
        data->entityClass.invokeMethod( instance, print_custom_message_func, &string_param );
    }

    void ScriptEngine::shutDown()
    {
        shutDownMono();
        delete data;
    }

    void ScriptEngine::initializeMono()
    {
        mono_set_assemblies_path( "mono/lib" );

        MonoDomain *root_domain = mono_jit_init( "SmileJITRuntime" );
        SM_ASSERT( root_domain, "ScriptEngine::initializeMono > Cannot initialize root domain" );
        
        // Store the root domain pointer
        data->rootDomain = root_domain;
    }

    void ScriptEngine::shutDownMono()
    {
        //mono_domain_unload( data->appDomain );
        data->appDomain = nullptr;

        //mono_jit_cleanup( data->rootDomain );
        data->rootDomain = nullptr;
    }

    void ScriptEngine::loadAssembly( const std::filesystem::path &file_path )
    {
        data->appDomain = mono_domain_create_appdomain( ( char * )"SmileScriptRuntime", nullptr );
        mono_domain_set( data->appDomain, true );

        data->coreAssembly = utils::loadMonoAssembly( file_path );
        data->coreAssemblyImage = mono_assembly_get_image( data->coreAssembly );
        // utils::printAssemblyTypes( data->coreAssembly );
    }

    MonoObject *ScriptEngine::instantiateClass( MonoClass *mono_class )
    {
        MonoObject *instance = mono_object_new( data->appDomain, mono_class );
        mono_runtime_object_init( instance );
        return instance;
    }

    ScriptClass::ScriptClass( const std::string &class_namespace, const std::string &class_name )
        : classNamespace{ class_namespace }, className{ class_name }
    {
        monoClass = mono_class_from_name( data->coreAssemblyImage, class_namespace.c_str(), class_name.c_str() );
    }

    MonoObject *ScriptClass::instantiate()
    {
        return ScriptEngine::instantiateClass( monoClass );
    }

    MonoMethod *ScriptClass::getMethod( const std::string &name, int parameter_count )
    {
        return mono_class_get_method_from_name( monoClass, name.c_str(), parameter_count );
    }

    MonoObject *ScriptClass::invokeMethod( MonoObject *instance, MonoMethod *method, void **params )
    {
        return mono_runtime_invoke( method, instance, params, nullptr );
    }
}