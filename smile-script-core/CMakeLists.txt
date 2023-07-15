file(GLOB_RECURSE SOURCE_FILES Source/*.cs)
project("smile-script-core" LANGUAGES CSharp)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME}
    PROPERTIES
    FOLDER Core
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/smile-editor/resources/scripts"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/smile-editor/resources/scripts"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/smile-editor/resources/scripts"
)