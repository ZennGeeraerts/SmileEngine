set(LIB_LINKS
    Ws2_32
    Winmm
    Version
    Bcrypt
)

target_link_libraries(smile PRIVATE ${LIB_LINKS})

file(GLOB_RECURSE SOURCE_FILES *.cpp)
target_sources(smile PRIVATE ${SOURCE_FILES})