cmake_minimum_required(VERSION 3.12)
project(Day_07_2)

set(CMAKE_CXX_STANDARD 14)

add_executable(Day_07_2 main.cpp IntcodeComputer.cpp IntcodeComputer.h Command.cpp Command.h)
configure_file(program.txt program.txt COPYONLY)
