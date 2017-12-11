set(CMAKE_CXX_STANDARD 11)

add_library(src STATIC
        counting.hpp
        enumerator.hpp
        arrayenumerator.hpp
        linsearch.hpp
        maxsearch.hpp
        procedure.hpp
        selection.hpp
        seqinfileenumerator.hpp
        summation.hpp
        main.cpp
        )

target_include_directories(src PUBLIC headers)