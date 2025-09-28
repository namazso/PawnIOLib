cmake_minimum_required(VERSION 3.25)

project(PawnIOLib)

add_library(${PROJECT_NAME} SHARED PawnIOLib.cpp)

target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)

target_link_libraries(${PROJECT_NAME} PRIVATE ntdll.lib)

target_link_options(${PROJECT_NAME} PRIVATE
        /ENTRY:DllEntry
        /NOCOFFGRPINFO
        /Brepro
        /PDBALTPATH:%_PDB%
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public)
