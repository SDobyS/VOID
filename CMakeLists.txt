cmake_minimum_required(VERSION 3.25)

project(VOID)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Stb REQUIRED)

add_library(glad
        extern/src/gl.c
)

target_include_directories(glad PUBLIC
        ${CMAKE_SOURCE_DIR}/extern/include
)

add_executable(VOID
        src/main.cpp
        src/stb.cpp
)

target_link_libraries(VOID PRIVATE
        SDL3::SDL3
        OpenGL::GL
        glad
)

target_include_directories(VOID PRIVATE
        ${Stb_INCLUDE_DIR}
)

set_target_properties(VOID PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)