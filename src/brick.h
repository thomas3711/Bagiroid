#pragma once
#include <SDL3/SDL.h>

class Brick
{
    SDL_FRect rectangle;
    SDL_Color color;
    bool alive = true;

public:
    const SDL_FRect GetRectangle() { return rectangle; };

    Brick(SDL_FPoint position, SDL_FPoint dimensions, SDL_Color color_p);

    void Render(SDL_Renderer* renderer);

    void Kill();
    bool IsAlive() { return alive; };
    void Respawn();
};