cmake_minimum_required(VERSION 3.12)
project(Day_17_2)

set(CMAKE_CXX_STANDARD 14)

add_executable(Day_17_2 main.cpp IntcodeComputer.cpp IntcodeComputer.h Command.cpp Command.h Screen.cpp Screen.h)
configure_file(program.txt program.txt COPYONLY)