cmake_minimum_required(VERSION 3.12)
project(Day_16_1)

set(CMAKE_CXX_STANDARD 14)

add_executable(Day_16_1 main.cpp)
configure_file(signals.txt signals.txt COPYONLY)