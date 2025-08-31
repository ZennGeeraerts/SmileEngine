include(smile)

include(build/yaml.cmake)

set(DIRECTORIES
    application
    asset
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