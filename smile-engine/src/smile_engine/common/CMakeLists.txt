include(smile-engine)

file(GLOB_RECURSE SOURCE_FILES *.cpp)
target_sources(smile-engine PRIVATE ${SOURCE_FILES})
target_include_directories(smile-engine PUBLIC .)