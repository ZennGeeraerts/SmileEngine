function(set_compiler_flags)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        add_compile_options(-g3 -O0 -Wall -pedantic -Wextra -m64 -mavx2 -mfma -ffast-math) #Using Clang
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        add_compile_options(-Wall -pedantic -Wextra -m64 -mavx2 -mfma -ffast-math) #Using GCC
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        add_compile_options(/std:c++17 /W4 /arch:AVX2 /fp:fast /MP /DNOMINMAX) #Using Visual Studio C++
    endif()
endfunction()