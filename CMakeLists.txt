cmake_minimum_required(VERSION 3.25)

project(VOID)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

add_library(glad
        extern/src/gl.c
)

target_include_directories(glad
        PUBLIC
        ${CMAKE_SOURCE_DIR}/extern/include
)

add_executable(VOID
        src/main.cpp
)

target_link_libraries(VOID
        PRIVATE
        SDL3::SDL3
        OpenGL::GL
        glad
)

set_target_properties(VOID PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)