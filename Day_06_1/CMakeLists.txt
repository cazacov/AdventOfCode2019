cmake_minimum_required(VERSION 3.12)
project(Day_06_1)

set(CMAKE_CXX_STANDARD 14)

add_executable(Day_06_1 main.cpp)
configure_file(orbits.txt orbits.txt COPYONLY)