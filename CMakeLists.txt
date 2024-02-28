cmake_minimum_required(VERSION 3.22.1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_FLAGS "-O3 -pg -g")
SET(CMAKE_EXE_LINKER_FLAGS "-O3 -pg -g")

project(49
        VERSION 3.0
        DESCRIPTION "Advanced Programming Project 49"
        LANGUAGES CXX)

add_executable(49 main.cpp src/BarnesHut.cpp src/CelestialBody.cpp src/TreeNode.cpp src/ConfigParser.cpp src/utils.cpp)

target_link_libraries(49 -fsanitize=address)