cmake_minimum_required(VERSION 3.25)

project(VOID)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(SDL3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)


add_executable(VOID
        src/main.cpp
)


target_link_libraries(VOID
        PRIVATE
        SDL3::SDL3
        OpenGL::GL
)

set_target_properties(VOID PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)