cmake_minimum_required(VERSION 3.9)
project(PasswordGues)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

add_executable(PasswordGues main.cpp)