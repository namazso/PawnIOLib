cmake_minimum_required(VERSION 3.25)

project(PawnIOUtil)

add_executable(${PROJECT_NAME} PawnIOUtil.cpp signature.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE PawnIOLib)

set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
