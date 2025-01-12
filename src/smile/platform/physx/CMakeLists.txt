include(smile)

find_package(PhysX REQUIRED)

include_directories (${PhysX_INCLUDE_DIRS})
target_link_libraries(smile PRIVATE physx::physx)

file(GLOB_RECURSE SOURCE_FILES *.cpp)
target_sources(smile PRIVATE ${SOURCE_FILES})