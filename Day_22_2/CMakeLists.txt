cmake_minimum_required(VERSION 3.12)
project(Day_22_2)

set(CMAKE_CXX_STANDARD 14)

add_executable(Day_22_2 main.cpp)
configure_file(input.txt input.txt COPYONLY)