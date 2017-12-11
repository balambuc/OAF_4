cmake_minimum_required(VERSION 3.2)
project(app)

set(CMAKE_CXX_STANDARD 11)

add_executable(app
        main.cpp)

target_link_libraries(app src)
