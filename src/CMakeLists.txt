set(CMAKE_CXX_STANDARD 11)

add_library(src STATIC
        enumerator.hpp
        procedure.hpp
        seqinfileenumerator.hpp
        summation.hpp
        main.cpp
        )

target_include_directories(src PUBLIC headers)