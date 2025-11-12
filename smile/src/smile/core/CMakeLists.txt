include(smile)

include(build/yaml.cmake)
include(build/serializer.cmake)

set(DIRECTORIES
    application
    asset
    commandline
    ecs
    geometric
    input
    math
    project
    window
    world
)

get_files_recursively(SOURCE_FILES ${DIRECTORIES})

target_sources(smile PRIVATE ${SOURCE_FILES})