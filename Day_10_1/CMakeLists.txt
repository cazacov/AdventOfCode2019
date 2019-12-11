cmake_minimum_required(VERSION 3.12)
project(Day_10_1)

set(CMAKE_CXX_STANDARD 14)

add_executable(Day_10_1 main.cpp)
configure_file(starmap.txt starmap.txt COPYONLY)
