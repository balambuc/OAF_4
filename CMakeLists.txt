cmake_minimum_required(VERSION 3.2)
project(bead4)

set(CMAKE_CXX_STANDARD 11) # C++11 standard

## Common flags
# Wall: verbose warning
# Weffc++: warnings based on Scott Meyers Effective c++
# g: make binary more debuggable
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Weffc++ -g")

if ($ENV{TRAVIS}) # On Travis-CI add coverage ability and disable optimizations
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -coverage")
    set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)
else () # Enable lvl2 optimizations else
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
endif ()

add_subdirectory(src)
add_subdirectory(app)

