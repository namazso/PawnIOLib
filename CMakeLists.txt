cmake_minimum_required(VERSION 3.25)

project(PawnIO)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(PawnIOLib)
add_subdirectory(PawnIOUtil)
