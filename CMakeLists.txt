cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)

project(MDGeom)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(demo_cpp ./src/demo.cpp)

target_include_directories(
demo_cpp PRIVATE ./inc
)

set(Boost_USE_STATIC_LIBS ON)

find_package(Boost COMPONENTS unit_test_framework)

if (Boost_FOUND)

add_executable(test_cpp ./src/test.cpp)

target_include_directories(
test_cpp PRIVATE ./inc
)

target_link_libraries(
test_cpp ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)

endif()
