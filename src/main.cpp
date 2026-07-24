#include <SDL3/SDL.h>
#include <glad/gl.h>
#include <stb_image.h>

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define FLIP_TEXTURE true

std::string ReadFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "[ERROR] Failed to open file: " << path << '\n';
        return {};
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

void Shutdown(SDL_Window* window, SDL_GLContext context) {
    if (context) SDL_GL_DestroyContext(context);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    std::cout << "=== VOID ===\n\n";
    std::cout << "[INFO] Starting...\n";

    // SDL3 Init
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "[ERROR] SDL could not initialize! SDL_Error: " << SDL_GetError() << '\n';
        return 1;
    }

    // OpenGL Attribute
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // Create Window with OpenGL
    SDL_Window* window = SDL_CreateWindow("VOID", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "[ERROR] Create window error: " << SDL_GetError() << '\n';
        SDL_Quit();
        return 1;
    }

    // OpenGL Context
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (!gl_context) {
        std::cerr << "[ERROR] Create gl_context error: " << SDL_GetError() << '\n';
        Shutdown(window, gl_context);
        return 1;
    }

    SDL_GL_MakeCurrent(window, gl_context);

    // Vsync
    SDL_GL_SetSwapInterval(1);

    // Glad2 Init
    if (!gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress)) {
        std::cerr << "[ERROR] Failed to initialize GLAD.\n";
        Shutdown(window, gl_context);
        return 1;
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // GPU info for test
    std::cout << "Vendor:   " << glGetString(GL_VENDOR) << '\n';
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << '\n';
    std::cout << "Version:  " << glGetString(GL_VERSION) << '\n';

    // Import files
    const std::string ASSET_PATH = "../assets/";
    std::string vertexSource = ReadFile(ASSET_PATH + "shaders/triangle.vert");
    std::string fragmentSource = ReadFile(ASSET_PATH + "shaders/triangle.frag");

    if (vertexSource.empty() || fragmentSource.empty()) {
        std::cerr << "[ERROR] Failed to load shader files.\n";
        Shutdown(window, gl_context);
        return 1;
    }

    const char* vertexCode = vertexSource.c_str();
    const char* fragmentCode = fragmentSource.c_str();

    // Vert AND Frag
    GLint success;
    char infoLog[1024];

    // Compilation Vertex Shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexCode, nullptr);
    glCompileShader(vertexShader);

    // Check Vertex Shader
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 1024, nullptr, infoLog);
        std::cerr << "[ERROR] Vertex shader compilation failed:\n" << infoLog << '\n';
        Shutdown(window, gl_context);
        return 1;
    }

    // Compilation Fragment Shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentCode, nullptr);
    glCompileShader(fragmentShader);

    // Check Fragment Shader
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 1024, nullptr, infoLog);
        std::cerr << "[ERROR] Fragment shader compilation failed:\n" << infoLog << '\n';
        Shutdown(window, gl_context);
        return 1;
    }

    // Create Program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check Link
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 1024, nullptr, infoLog);
        std::cerr << "[ERROR] Shader program linking failed:\n" << infoLog << '\n';
        Shutdown(window, gl_context);
        return 1;
    }

    // Delete Shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // ImGui Init
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    if (!ImGui_ImplSDL3_InitForOpenGL(window, gl_context)) {
        std::cerr << "[ERROR] Failed to initialize ImGui SDL3 backend\n";
        Shutdown(window, gl_context);
        return 1;
    }
    ImGui_ImplOpenGL3_Init("#version 460");

    float vertices[] = {
        // pos           // color               // texture coord
        -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
        -0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 0.0f,
         0.5f, -0.5f,    0.0f,  0.0f,  1.0f,    1.0f, 0.0f,
         0.5f,  0.5f,    1.0f,  1.0f,  0.0f,    1.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    // VAO VBO EBO STB
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    GLuint texture;

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

    // Texture

    // Flip texture
    stbi_set_flip_vertically_on_load(FLIP_TEXTURE);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Parameteri
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    /*
        Default Texture:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        Pixel Texture:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    */

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int texture_width, texture_height, nrChannels;
    unsigned char* data = stbi_load(
        (ASSET_PATH + "textures/test.png").c_str(),
        &texture_width,
        &texture_height,
        &nrChannels,
        0
    );

    if (data) {
        std::cout << "[OK] Texture: \"../assets/textures/test.png\" loaded successfully.\n";

        // Format RGB or RGBA
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;

        glTexImage2D(GL_TEXTURE_2D, 0, format, texture_width, texture_height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    } else {
        std::cerr << "[ERROR] Could not load texture: \"../assets/textures/test.png\". Using fallback checkerboard.\n";

        unsigned char fallbackPixels[4 * 4] = {
            255, 0, 255, 255,  // 0 0 purple
              0, 0,   0, 255,  // 1 0 black
              0, 0,   0, 255,  // 0 1 black
            255, 0, 255, 255   // 1 1 purple
        };

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            2,
            2,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            fallbackPixels
        );
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    // Main
    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT) // exit
                running = false;
        }
        int w, h;
        SDL_GetWindowSize(window, &w, &h);
        glViewport(0, 0, w, h);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow();
        ImGui::Render();

        // Shader
        glUseProgram(shaderProgram);

        // Uniform
        float aspect = static_cast<float>(h) / static_cast<float>(w);
        glUniform1f(glGetUniformLocation(shaderProgram, "aspect"), aspect);

        // Drawing
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(vao);

        glDrawElements(
            GL_TRIANGLES,
            6,
            GL_UNSIGNED_INT,
            nullptr
        );

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    // Clear
    // VAO VBO EBO STB
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteTextures(1, &texture);

    // ShaderProgram
    glDeleteProgram(shaderProgram);

    // ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    // SDL3
    SDL_GL_DestroyContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}