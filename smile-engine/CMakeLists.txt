set(CMAKE_MFC_FLAG 1)

file(GLOB_RECURSE SOURCE_FILES
    src/*.cpp
    src/*.hpp
)

if(NOT WITH_SMILERASTER)
    message("Not with SmileRaster")
    file(GLOB_RECURSE SMILERASTER_ITEMS
    src/platform/smileraster/*.cpp
    src/platform/smileraster/*.hpp
    )

    set(source_files ${SOURCE_FILES})
    foreach(item in ${SMILERASTER_ITEMS})
        list(REMOVE_ITEM ${SOURCE_FILES} item)
        message(${item})
    endforeach()
    #list(REMOVE_ITEM source_files ${SMILERASTER_ITEMS})
endif()

message(${SOURCE_FILES})

add_library(smile-engine STATIC ${SOURCE_FILES})

if(WIN32)
    target_compile_definitions( smile-engine PUBLIC SM_PLATFORM_WINDOWS )
endif()

conan_cmake_run(
        REQUIRES
            physx/4.1.1
            assimp/5.2.2
            yaml-cpp/0.7.0
            entt/3.10.1
            imguizmo/cci.20210223
            sdl/2.0.20
            sdl_image/2.0.5
        BASIC_SETUP CMAKE_TARGETS
        BUILD missing
        )

if(TARGET CONAN_PKG::physx)
    target_link_libraries(smile-engine PRIVATE CONAN_PKG::physx)
endif()

if(TARGET CONAN_PKG::assimp)
    target_link_libraries(smile-engine PRIVATE CONAN_PKG::assimp)
endif()

if(TARGET CONAN_PKG::yaml-cpp)
    target_link_libraries(smile-engine PRIVATE CONAN_PKG::yaml-cpp)
endif()

if(TARGET CONAN_PKG::entt)
    target_link_libraries(smile-engine PRIVATE CONAN_PKG::entt)
endif()

if(TARGET CONAN_PKG::imguizmo)
    target_link_libraries(smile-engine PRIVATE CONAN_PKG::imguizmo)
endif()

if(WITH_SMILERASTER)
    if(TARGET CONAN_PKG::sdl)
        target_link_libraries(smile-engine PRIVATE CONAN_PKG::sdl)
    endif()
    if(TARGET CONAN_PKG::sdl_image)
        target_link_libraries(smile-engine PRIVATE CONAN_PKG::sdl_image)
    endif()
endif()

target_link_libraries(smile-engine PRIVATE smile-raster)

target_include_directories(smile-engine
                            PUBLIC .
                            PRIVATE $<$<CONFIG:Release>:${CMAKE_CURRENT_BINARY_DIR}>
                            PRIVATE ${CMAKE_SOURCE_DIR}/smile-engine/src
                            PUBLIC smile-raster/src
                            PUBLIC ${CMAKE_SOURCE_DIR}/thirdparty/dx11effects/include
                            PUBLIC ${CMAKE_SOURCE_DIR}/thirdparty/dx11tex/include)

target_link_directories(smile-engine
                            PUBLIC ${CMAKE_SOURCE_DIR}/thirdparty/dx11effects/lib
                            PUBLIC PUBLIC ${CMAKE_SOURCE_DIR}/thirdparty/dx11tex/lib)

add_definitions(-DUNICODE)

message(STATUS "Configuring SmileEngine... done!")