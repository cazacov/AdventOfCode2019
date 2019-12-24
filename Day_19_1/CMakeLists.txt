cmake_minimum_required(VERSION 3.12)
project(Day_19_1)

set(CMAKE_CXX_STANDARD 14)

add_executable(Day_19_1 main.cpp IntcodeComputer.cpp IntcodeComputer.h Command.cpp Command.h Screen.cpp Screen.h)
configure_file(program.txt program.txt COPYONLY)