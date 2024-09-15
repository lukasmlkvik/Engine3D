cmake_minimum_required(VERSION 3.10)
project(Engine3D VERSION 1.0.0)

find_package(OpenGL REQUIRED)

add_executable(Engine3D
    src/main.cpp 
    externalLibs/glad/glad.c
)

set(GLFW_DIR ${CMAKE_CURRENT_SOURCE_DIR}/externalLibs/glfw-3.4.bin.WIN64)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/externalLibs/glfw-3.4)

target_include_directories(Engine3D 
    PRIVATE
    #${GLFW_DIR}/include
    externalLibs
)

target_link_libraries(Engine3D
    glfw
    OpenGL::GL 
)
