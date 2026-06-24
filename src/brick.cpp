#include "brick.h"
#include "game.h"

Brick::Brick()
{

}

Brick::Brick(SDL_FPoint position, SDL_FPoint dimensions, SDL_Color color_p, uint32_t points_p, SDL_Point id_p)
{
    rectangle.x = position.x;
    rectangle.y = position.y;
    rectangle.w = dimensions.x;
    rectangle.h = dimensions.y;
    color = color_p;
    points = points_p;
    id = id_p;
}

void Brick::Render(SDL_Renderer* renderer)
{
    if(active)
    {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(renderer, &rectangle);
    }
}

void Brick::Update(float delta_time)
{

}

void Brick::Kill()
{
    SetActive(false);
    Game::GetInstance()->NotifyBrickDestruction(this);
}

void Brick::Reset()
{
    if(powerup)
    {
        delete powerup;
        powerup = nullptr;
    }
    
    SetActive(true);
}