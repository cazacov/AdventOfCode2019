cmake_minimum_required(VERSION 3.12)
project(Day_02_01)

set(CMAKE_CXX_STANDARD 14)

add_executable(Day_02_01 main.cpp IntcodeComputer.cpp IntcodeComputer.h)
configure_file(program.txt program.txt COPYONLY)