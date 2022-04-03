cmake_minimum_required(VERSION 3.21)
project(Wordle__)

set(CMAKE_CXX_STANDARD 23)

configure_file(resources)
add_executable(Wordle__ src/main.cpp)