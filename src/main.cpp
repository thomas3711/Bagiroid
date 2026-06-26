#include <SDL3/SDL.h>
#include "game.h"

#define APPLICATION_NAME "Bagiroid"

SDL_Renderer* renderer;
SDL_Window* window;

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    // Enable plugin loading only when started with -p (or --plugins).
    bool load_plugins = false;
    for (int i = 1; i < argc; i++)
    {
        if (SDL_strcmp(argv[i], "-p") == 0 || SDL_strcmp(argv[i], "--plugins") == 0)
        {
            load_plugins = true;
        }
    }

    window = SDL_CreateWindow(APPLICATION_NAME, Game::GetInstance()->window_width, Game::GetInstance()->window_height, 0);
    renderer = SDL_CreateRenderer(window, nullptr);

    Game::GetInstance()->Initialize(load_plugins);

    bool running = true;
    SDL_Event event;

    Uint64 last_frame_time = SDL_GetTicks();

    while (running) {
        // TODO: Encapsulate
        // Calculate delta time in seconds
        Uint64 current_frame_time = SDL_GetTicks();
        float delta_time = (current_frame_time - last_frame_time) / 1000.0f;
        last_frame_time = current_frame_time;

        // TODO: Encapsulate
        // Check application quit
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT) running = false;
        }

        // Update
        Game::GetInstance()->Update(delta_time);

        // Render
        Game::GetInstance()->Render(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}