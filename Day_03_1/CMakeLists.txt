cmake_minimum_required(VERSION 3.10)
project(Day_03_1)

set(CMAKE_CXX_STANDARD 14)

add_executable(Day_03_1 main.cpp)
configure_file(wires.txt wires.txt COPYONLY)