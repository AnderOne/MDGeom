cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)

project(MDGeom)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(demo ./src/demo.cpp)

target_include_directories(
demo PRIVATE ./inc
)
