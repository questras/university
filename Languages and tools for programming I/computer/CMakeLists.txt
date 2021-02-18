cmake_minimum_required(VERSION 3.17)
project(computer)

set(CMAKE_CXX_STANDARD 17)

add_executable(computer computer_test.cc computer.h)