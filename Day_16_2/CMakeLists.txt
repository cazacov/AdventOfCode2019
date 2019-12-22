cmake_minimum_required(VERSION 3.12)
project(Day_16_2)

set(CMAKE_CXX_STANDARD 14)

add_executable(Day_16_2 main.cpp)
configure_file(signals.txt signals.txt COPYONLY)