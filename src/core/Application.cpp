#include "Application.h"
#include "Config.h"

#include "../graphics/Shader.h"
#include "../graphics/Texture.h"
#include "../graphics/Camera/OrthographicCamera.h"
#include "../utils/Log.h"
#include "../utils/SystemInfo.h"

#include <SDL3/SDL.h>
#include <glad/gl.h>

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Application::Application() {}
Application::~Application() {}

int Application::Run() {
    Log::Info("Starting VOID Engine...");

    // SDL3 Init
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        Log::Error("SDL initialization failed", SDL_GetError());
        return 1;
    }
    Log::Success("SDL initialized");

    // OpenGL Attribute
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // Create window with OpenGL
    SDL_Window* window = SDL_CreateWindow(
        "VOID",
        Config::WindowWidth,
        Config::WindowHeight,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    if (!window) {
        Log::Error("Create window error", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    Log::Success("Window created successfully");

    // OpenGL Context
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (!gl_context) {
        Log::Error("Create gl_context error", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    Log::Success("OpenGL context created successfully");

    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(Config::VSyncEnabled); // VSyns

    // Glad2 Init
    if (!gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress)) {
        Log::Error("Failed to initialize GLAD");
        SDL_GL_DestroyContext(gl_context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    Log::Success("GLAD initialized");

    // Blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Info
    SystemInfo::PrintHardwareInfo();
    SystemInfo::PrintGPUInfo();

    // Import files
    Shader shader(
        Config::AssetPath + "shaders/triangle.vert",
        Config::AssetPath + "shaders/triangle.frag"
    );

    if (shader.GetID() == 0) {
        Log::Error("Shader initialization failed", "Check shader files");
        SDL_GL_DestroyContext(gl_context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    Log::Success("Shader loaded");

    Texture texture(Config::AssetPath + "textures/test.png");
    Log::Success("Texture processed");

    // Camera
    OrthographicCamera camera(
        0.0f,
        static_cast<float>(Config::WindowWidth),
        static_cast<float>(Config::WindowHeight),
        0.0f
    );

    // ImGui Init
    IMGUI_CHECKVERSION(); ImGui::CreateContext();
    if (!ImGui_ImplSDL3_InitForOpenGL(window, gl_context)) {
        Log::Error("Failed to initialize ImGui SDL3 backend");
        SDL_GL_DestroyContext(gl_context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    ImGui_ImplOpenGL3_Init("#version 460");
    Log::Success("ImGui initialized");

    // Vertices
    float vertices[] = {
        // pos             // color             // texture coord
        100.0f, 200.0f,    1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
        100.0f, 100.0f,    0.0f, 1.0f, 0.0f,    0.0f, 1.0f,
        200.0f, 100.0f,    0.0f, 0.0f, 1.0f,    1.0f, 1.0f,
        200.0f, 200.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    // VAO VBO EBO
    GLuint vao, vbo, ebo;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    // VAO
    glBindVertexArray(vao);

    // VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );

    // EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(indices),
        indices,
        GL_STATIC_DRAW
    );

    // pos (vec2)
    glVertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        7 * sizeof(float),
        nullptr
    );
    glEnableVertexAttribArray(0);

    // color (vec3)
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        7 * sizeof(float),
        reinterpret_cast<void*>(2 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    // texture coord (vec2)
    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        7 * sizeof(float),
        reinterpret_cast<void*>(5 * sizeof(float))
    );
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    /*
     *
     * MAIN
     *
     */

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT) running = false; // Exit
        }

        int w, h;
        SDL_GetWindowSize(window, &w, &h);
        glViewport(0, 0, w, h);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Camera
        const bool* keys = SDL_GetKeyboardState(nullptr);
        glm::vec3 pos = camera.GetPosition();

        if (keys[SDL_SCANCODE_W]) pos.y -= Config::CameraSpeed;
        if (keys[SDL_SCANCODE_S]) pos.y += Config::CameraSpeed;
        if (keys[SDL_SCANCODE_A]) pos.x -= Config::CameraSpeed;
        if (keys[SDL_SCANCODE_D]) pos.x += Config::CameraSpeed;

        camera.SetPosition(pos);

        // Drawing
        shader.Bind();
        shader.SetUniformMat4f("u_ViewProj", glm::value_ptr(camera.GetViewProjectionMatrix()));
        texture.Bind();
        glBindVertexArray(vao);

        glDrawElements(
            GL_TRIANGLES,
            6,
            GL_UNSIGNED_INT,
            nullptr
        );

        // Render ImGui
        ImGui_ImplOpenGL3_NewFrame(); ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame(); ImGui::ShowDemoWindow(); ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(window);
    }

    // Cleanup

    // VAO VBO EBO
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    // ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    // SDL3
    SDL_GL_DestroyContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    Log::Info("VOID Engine shut down successfully.");
    return 0;
}
