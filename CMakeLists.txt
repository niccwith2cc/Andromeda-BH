cmake_minimum_required(VERSION 3.22.1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(49
        VERSION 2.0
        DESCRIPTION "Advanced Programming Project 49"
        LANGUAGES CXX)

add_executable(49 main.cpp BarnesHut.cpp CelestialBody.cpp TreeNode.cpp)

target_compile_options(49 PRIVATE -Wall)