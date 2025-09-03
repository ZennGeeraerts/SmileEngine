include(smile)

if(COMMAND add_serializer)
    add_serializer(physics)
endif()

file(GLOB_RECURSE SOURCE_FILES *.cpp)
target_sources(smile PRIVATE ${SOURCE_FILES})