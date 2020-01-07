cmake_minimum_required(VERSION 3.12)
project(Day25)

set(CMAKE_CXX_STANDARD 14)

add_executable(Day25 main.cpp Command.h Command.cpp IntcodeComputer.h IntcodeComputer.cpp Screen.h Screen.cpp AsciiComputer.cpp AsciiComputer.h)
configure_file(program.txt program.txt COPYONLY)