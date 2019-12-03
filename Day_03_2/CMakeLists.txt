cmake_minimum_required(VERSION 3.16)
project(Day_03_2)

set(CMAKE_CXX_STANDARD 14)

add_executable(Day_03_2 main.cpp)
configure_file(wires.txt wires.txt COPYONLY)