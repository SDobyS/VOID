#include <SDL3/SDL.h>
#include <glad/gl.h>
#include <iostream>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

int main(int argc, char* argv[]) {
    // SDL3 Init
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << '\n';
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
        std::cout << "Create window error: " << SDL_GetError() << '\n';
        SDL_Quit();
        return 1;
    }

    // OpenGL Context
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (!gl_context) {
        std::cout << "Create gl_context error: " << SDL_GetError() << '\n';
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_GL_MakeCurrent(window, gl_context);

    // Vsync
    SDL_GL_SetSwapInterval(1);

    // Glad2 Init
    if (!gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress)) {
        std::cout << "could not initialize! Glad error: " << SDL_GetError() << '\n';
        SDL_GL_DestroyContext(gl_context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // GPU info for test
    std::cout << "Vendor:   " << glGetString(GL_VENDOR) << '\n';
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << '\n';
    std::cout << "Version:  " << glGetString(GL_VERSION) << '\n';

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            // Exit
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Update Window
        SDL_GL_SwapWindow(window);
    }

    // Clear
    SDL_GL_DestroyContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
