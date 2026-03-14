include(smile)

include(build/yaml.cmake)
include(build/serializer.cmake)

set(DIRECTORIES
    application
    asset
    commandline
    ecs
    fs
    geometric
    input
    math
    project
    signal
    window
    world
)

get_files_recursively(SOURCE_FILES ${DIRECTORIES})

target_sources(smile PRIVATE ${SOURCE_FILES})