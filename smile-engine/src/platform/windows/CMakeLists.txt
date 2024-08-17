include(smile-engine)

set(LIB_LINKS
    Ws2_32
    Winmm
    Version
    Bcrypt
)

target_link_libraries(smile-engine PRIVATE ${LIB_LINKS})

file(GLOB_RECURSE SOURCE_FILES *.cpp)
target_sources(smile-engine PRIVATE ${SOURCE_FILES})