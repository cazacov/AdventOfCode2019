cmake_minimum_required(VERSION 3.10)
project(Day_01_1)

set(CMAKE_CXX_STANDARD 14)

add_executable(Day_01_1 main.cpp)
configure_file(module_masses.txt module_masses.txt COPYONLY)