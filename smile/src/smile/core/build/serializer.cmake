set_property(GLOBAL PROPERTY serializer_namespaces "")

function( add_serializer namespace )

    get_property(namespace_list GLOBAL PROPERTY serializer_namespaces)

    list(APPEND namespace_list ${namespace})

    set_property(GLOBAL PROPERTY serializer_namespaces "${namespace_list}")

endfunction()

set(generate_serializer_file__internal_dir ${CMAKE_CURRENT_SOURCE_DIR}/serializer CACHE INTERNAL "")

macro( generate_serializer_file target )

    get_property(namespace_list GLOBAL PROPERTY serializer_namespaces)

    set(serializer_code "")
    set(deserializer_code "")

    foreach( namespace ${namespace_list} )
        set(forward_definition "${forward_definition}namespace smile::${namespace}::serializer{ void Serialize( yaml::Emitter &, world::Entity ); void Deserialize( const yaml::Node &, world::Entity ); }\n")
        set(serializer_code "${serializer_code}    smile::${namespace}::serializer::Serialize( output, entity );\n")
        set(deserializer_code "${deserializer_code}    smile::${namespace}::serializer::Deserialize( data, entity );\n")
    endforeach()

    configure_file(
        ${generate_serializer_file__internal_dir}/external_serialize_function.cpp.in
        external_serialize_function.cpp
    )

    target_sources( ${target} PUBLIC external_serialize_function.cpp )

endmacro()

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "serializer/*.cpp")

target_sources(smile PRIVATE ${SOURCE_FILES})