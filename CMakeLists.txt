cmake_minimum_required(VERSION 3.25)
project(VOID LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

find_package(OpenGL REQUIRED)

include(FetchContent)

# SDL3
FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG        release-3.2.0
)
set(SDL_TEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(SDL3)

# GLAD 2
FetchContent_Declare(
        glad
        GIT_REPOSITORY https://github.com/Dav1dde/glad.git
        GIT_TAG        v2.0.6
)

FetchContent_MakeAvailable(glad)

add_subdirectory(
        ${glad_SOURCE_DIR}/cmake
        glad_cmake
)

glad_add_library(
        glad
        REPRODUCIBLE
        API gl:core=4.6
)

# stb_image
FetchContent_Declare(
        stb
        GIT_REPOSITORY https://github.com/nothings/stb.git
        GIT_TAG        master
)
FetchContent_MakeAvailable(stb)

# GLM
FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG        1.0.1
)
FetchContent_MakeAvailable(glm)


# Dear ImGui
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG        v1.91.5
)
FetchContent_MakeAvailable(imgui)

add_library(imgui STATIC
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
)

target_link_libraries(imgui PUBLIC SDL3::SDL3 OpenGL::GL)

set(ENTRY_FILE "${CMAKE_CURRENT_SOURCE_DIR}/test.txt")

if(EXISTS "${ENTRY_FILE}")
    file(READ "${ENTRY_FILE}" CUSTOM_PATH)
    string(STRIP "${CUSTOM_PATH}" MAIN_SOURCE_FILE)
    message(STATUS "==> Building from specified path: ${MAIN_SOURCE_FILE}")
else()
    message(FATAL_ERROR "==> File 'test.txt' not found! Create it and specify the path to your main.cpp.")
endif()

add_executable(VOID
        # Your Project
        ${MAIN_SOURCE_FILE}

        # Libs
        src/engine/Window.cpp
        src/engine/ImGuiLayer.cpp
        src/engine/stb.cpp
        src/engine/Input.cpp

        # Graphics
        src/engine/graphics/Mesh.cpp
        src/engine/graphics/Shader.cpp
        src/engine/graphics/Texture.cpp
        src/engine/graphics/VertexArray.cpp
        src/engine/graphics/Camera/OrthographicCamera.cpp
        src/engine/graphics/Renderer.cpp
        src/engine/graphics/Sprite.cpp

        # Utils
        src/engine/utils/Log.cpp
        src/engine/utils/SystemInfo.cpp
        src/engine/utils/Time.cpp
        src/engine/WindowsGPU.cpp
)

target_link_libraries(VOID PRIVATE
        SDL3::SDL3
        OpenGL::GL
        glad
        imgui
        glm
)

target_include_directories(VOID PRIVATE
        ${stb_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/engine
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(VOID PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)