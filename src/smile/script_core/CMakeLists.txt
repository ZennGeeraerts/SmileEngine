file(GLOB_RECURSE SOURCE_FILES Source/*.cs)
project("smile-script-core" LANGUAGES CSharp)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME}
    PROPERTIES
    FOLDER Core
    ARCHIVE_OUTPUT_DIRECTORY "${ENGINE_PATH}/../../resources/scripts"
    LIBRARY_OUTPUT_DIRECTORY "${ENGINE_PATH}/../../resources/scripts"
    RUNTIME_OUTPUT_DIRECTORY "${ENGINE_PATH}/../../resources/scripts"
)