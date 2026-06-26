#include <SDL3/SDL.h>
#include "game.h"

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    auto game = Game::GetInstance();

    game->Initialize(argc, argv);

    while (game->IsApplicationRunning())
    {
        game->GetInstance()->Update();
        game->GetInstance()->Render();
    }

    game->GetInstance()->Quit();

    return 0;
}