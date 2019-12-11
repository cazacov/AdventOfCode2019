cmake_minimum_required(VERSION 3.12)
project(Day_10_2)

set(CMAKE_CXX_STANDARD 14)

add_executable(Day_10_2 main.cpp)
configure_file(starmap.txt starmap.txt COPYONLY)
