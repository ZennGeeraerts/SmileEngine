list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})

include(smile.cmake)

set(INCLUDE_DIRS
    ${CMAKE_CURRENT_LIST_DIR}
    ${THIRDPARTY_PATH}
)

if (NOT TARGET smile)
    add_library(smile STATIC)
endif()

target_compile_features(smile PUBLIC cxx_std_20)

target_include_directories(smile PUBLIC ${INCLUDE_DIRS})

target_precompile_headers(smile PUBLIC "smpch.h")

set_property(TARGET smile PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
set_target_properties(smile PROPERTIES FOLDER Core)

target_compile_definitions(
    smile
    PUBLIC
    $<$<CONFIG:Debug>:SM_C_DEBUG>
    $<$<CONFIG:Release>:SM_C_RELEASE>
)

add_definitions(-DUNICODE)

add_subdirectory(tools)