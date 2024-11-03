cmake_minimum_required(VERSION 3.1)
project(MyProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")

add_executable(main.exe main.cpp)
