#include "brick.h"
#include "game.h"

Brick::Brick(SDL_FPoint position, SDL_FPoint dimensions, SDL_Color color_p, uint32_t points_p)
{
    rectangle.x = position.x;
    rectangle.y = position.y;
    rectangle.w = dimensions.x;
    rectangle.h = dimensions.y;
    color = color_p;
    points = points_p;
}

void Brick::Render(SDL_Renderer* renderer)
{
    if(alive)
    {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(renderer, &rectangle);
    }
}

void Brick::Kill()
{
    alive = false;
    Game::GetInstance()->NotifyBrickDestruction(this);
}

void Brick::Respawn()
{
    alive = true;
}