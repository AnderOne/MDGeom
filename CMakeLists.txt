cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)

project(MDGeom)

add_executable(demo-1 ./src/demo-1.cpp)
add_executable(demo-2 ./src/demo-2.cpp)
add_executable(demo-3 ./src/demo-3.cpp)
add_executable(demo-4 ./src/demo-4.cpp)

target_include_directories(
demo-1 PRIVATE ./inc
)
target_include_directories(
demo-2 PRIVATE ./inc
)
target_include_directories(
demo-3 PRIVATE ./inc
)
target_include_directories(
demo-4 PRIVATE ./inc
)
