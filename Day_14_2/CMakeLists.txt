cmake_minimum_required(VERSION 3.12)
project(Day_14_2)

set(CMAKE_CXX_STANDARD 14)

add_executable(Day_14_2 main.cpp)
configure_file(reactions.txt reactions.txt COPYONLY)