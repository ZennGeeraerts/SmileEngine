set(SHADER_SOURCE_DIR ${SMILE_PATH}/smile/graphic/renderer)

set(SHADER_OUTPUT_DIR ${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/resources/shaders)

file(MAKE_DIRECTORY ${SHADER_OUTPUT_DIR})

file(GLOB_RECURSE SHADER_FILES
    "${SHADER_SOURCE_DIR}/*.hlsl"
)

set(COMPILED_SHADERS)

foreach(SHADER ${SHADER_FILES})
    get_filename_component(SHADER_FILENAME ${SHADER} NAME)
    string(REPLACE ".hlsl" ".smshader" SHADER_OUTPUT_FILE ${SHADER_FILENAME})
    set(SHADER_OUTPUT "${SHADER_OUTPUT_DIR}/${SHADER_OUTPUT_FILE}")

    add_custom_command(
        OUTPUT ${SHADER_OUTPUT}
        COMMAND $<TARGET_FILE:x-shader-c> ${SHADER} -o ${SHADER_OUTPUT}
        DEPENDS ${SHADER} x-shader-c
        COMMENT "Compiling shader ${SHADER_FILENAME}"
        VERBATIM
    )

    list(APPEND COMPILED_SHADERS ${SHADER_OUTPUT})
endforeach()

add_custom_target(compile-shaders ALL
    DEPENDS ${COMPILED_SHADERS}
)

add_dependencies(smile compile-shaders)