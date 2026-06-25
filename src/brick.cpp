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

void Brick::Destroy()
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

SDL_Color Brick::GetBrickColor(int row)
{
    SDL_Color color = { .r = 255, .g = 255, .b = 255, .a = 255 };

    switch (row)
    {
    case 0:
        color = {255, 0, 0, 255};
        break; // Red
    case 1:
        color = {255, 127, 0, 255};
        break; // Orange
    case 2:
        color = {255, 255, 0, 255};
        break; // Yellow
    case 3:
        color = {0, 255, 0, 255};
        break; // Green
    case 4:
        color = {0, 0, 255, 255};
        break; // Blue
    case 5:
        color = {128, 0, 128, 255};
        break; // Purple
    default:
        color = {255, 255, 255, 255};
        break;
    }

    return color;
}

int Brick::GetBrickPoints(int row)
{
    // TODO: there is probably some math solution (one line of code)
    int points = 0;

    switch (row)
    {
    case 0:
        points = 32;
        break; // Red
    case 1:
        points = 16;
        break; // Orange
    case 2:
        points = 8;
        break; // Yellow
    case 3:
        points = 4;
        break; // Green
    case 4:
        points = 2;
        break; // Blue
    case 5:
        points = 1;
        break; // Purple
    default:
        points = 0;
        break;
    }

    return points;
}