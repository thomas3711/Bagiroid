#include "brick.h"

Brick::Brick(SDL_FPoint position, SDL_FPoint dimensions, SDL_Color color_p)
{
    rectangle.x = position.x;
    rectangle.y = position.y;
    rectangle.w = dimensions.x;
    rectangle.h = dimensions.y;
    color = color_p;
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
}

void Brick::Respawn()
{
    alive = true;
}