cmake_minimum_required(VERSION 3.16)
project(Day_05_1)

set(CMAKE_CXX_STANDARD 14)

add_executable(Day_05_1 main.cpp IntcodeComputer.cpp IntcodeComputer.h Command.cpp Command.h)
configure_file(program.txt program.txt COPYONLY)