#include <SDL3/SDL.h>
#include "game.h"
#include "paddle.h"
#include "ball.h"

#define APPLICATION_NAME "Bagiroid"

void initialize_brick_field();
void draw_bricks();

SDL_Renderer* renderer;
SDL_Window* window;

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(APPLICATION_NAME, Game::GetInstance()->window_width, Game::GetInstance()->window_height, 0);
    renderer = SDL_CreateRenderer(window, nullptr);

    Game::GetInstance()->Initialize();

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
        while (SDL_PollEvent(&event)) {
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