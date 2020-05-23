cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)

project(MDGeom)

add_executable(demo_cpp ./src/demo.cpp)

target_include_directories(
demo_cpp PRIVATE ./inc
)
