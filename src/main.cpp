#include <SDL3/SDL.h>
#include "game.h"

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    auto game = Game::GetInstance();

    game->Initialize(argc, argv);

    while (game->IsApplicationRunning())
    {
        game->Update();
        game->Render();
    }

    game->Quit();

    return 0;
}