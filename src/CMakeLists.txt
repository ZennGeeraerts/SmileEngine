list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})

include(smile-engine.cmake)

set(INCLUDE_DIRS
    .
    ${CMAKE_SOURCE_DIR}/thirdparty)

if (NOT TARGET smile-engine)
    add_library(smile-engine STATIC)
endif()

target_include_directories(smile-engine PUBLIC ${INCLUDE_DIRS})

target_precompile_headers(smile-engine PUBLIC "smpch.h")

set_property(TARGET smile-engine PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
set_target_properties(smile-engine PROPERTIES FOLDER Core)

target_compile_definitions(
    smile-engine
    PUBLIC
    $<$<CONFIG:Debug>:SM_C_DEBUG>
    $<$<CONFIG:Release>:SM_C_RELEASE>
)

add_definitions(-DUNICODE)