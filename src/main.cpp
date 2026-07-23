#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <iostream>

int main(int argc, char* argv[]) {
    // SDL3 init
    if (!SDL_Init(SDL_INIT_VIDEO)) {

        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << '\n';

        return 1;

    }

    // OpenGL Attribute
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Create Window with OpenGL
    SDL_Window* window = SDL_CreateWindow("VOID", 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
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
