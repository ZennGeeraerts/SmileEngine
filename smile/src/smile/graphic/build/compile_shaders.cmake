smile_option(COMPILE_SHADERS "Use shader compilation" ON)

if(SMILE_COMPILE_SHADERS)
    set(COMPILED_SHADERS)

    set(SHADER_SOURCE_DIR ${SMILE_PATH}/smile/graphic/shader)

    set(SHADER_OUTPUT_DIR ${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/resources/shaders)

    file(MAKE_DIRECTORY ${SHADER_OUTPUT_DIR})

    file(GLOB_RECURSE SHADER_FILES
        "${SHADER_SOURCE_DIR}/*.hlsl"
    )

    foreach(SHADER ${SHADER_FILES})
        get_filename_component(SHADER_FILENAME ${SHADER} NAME)
        string(REPLACE ".hlsl" ".smshader" SHADER_OUTPUT_FILE ${SHADER_FILENAME})
        set(SHADER_OUTPUT "${SHADER_OUTPUT_DIR}/${SHADER_OUTPUT_FILE}")

        add_custom_command(
            OUTPUT ${SHADER_OUTPUT}
            COMMAND $<TARGET_FILE:x-shader-c> -i ${SHADER} -o ${SHADER_OUTPUT}
            DEPENDS ${SHADER} x-shader-c
            COMMENT "Compiling shader ${SHADER_FILENAME}"
            VERBATIM
        )

        list(APPEND COMPILED_SHADERS ${SHADER_OUTPUT})
    endforeach()

    add_custom_target(compile-shaders ALL
        DEPENDS ${COMPILED_SHADERS}
    )

    add_dependencies(${APPLICATION_TARGET} compile-shaders)
endif()