#pragma once
#include <SDL3/SDL.h>

class Brick
{
    SDL_FRect rectangle;
    SDL_Color color;
    bool alive = true;
    uint32_t points = 0;

public:
    const SDL_FRect& GetRectangle() { return rectangle; };

    Brick(SDL_FPoint position, SDL_FPoint dimensions, SDL_Color color_p, uint32_t points_p);

    void Render(SDL_Renderer* renderer);

    void Kill();
    bool IsAlive() { return alive; };
    void Respawn();
    uint32_t GetPoints() { return points; };
};