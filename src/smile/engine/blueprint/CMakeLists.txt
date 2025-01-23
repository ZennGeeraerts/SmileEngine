include(smile)

file(GLOB_RECURSE SOURCE_FILES *.cpp)
target_sources(smile PRIVATE ${SOURCE_FILES})
target_include_directories(smile PUBLIC .)