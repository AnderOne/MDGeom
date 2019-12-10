cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)

project(MDGeom)

file(GLOB GEOM_SRC "./src/geom/*.cpp")

add_library(geom ${GEOM_SRC})

target_include_directories(geom PRIVATE ./inc)

add_executable(demo_cpp ./src/demo.cpp)

target_include_directories(
demo_cpp PRIVATE ./inc
)

add_executable(demo_c ./src/demo.c)

target_include_directories(
demo_c PRIVATE ./inc
)

target_link_libraries(
demo_c
geom
)
