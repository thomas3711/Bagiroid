#pragma once
#include <SDL3/SDL.h>
#include "powerup.h"

class Brick
{
    SDL_FRect rectangle;
    SDL_Color color;
    bool alive = true;
    uint32_t points = 0;
    Powerup* powerup = nullptr;
    SDL_Point id;

public:
    Brick(SDL_FPoint position, SDL_FPoint dimensions, SDL_Color color_p, uint32_t points_p, SDL_Point id_p);


    const SDL_FRect& GetRectangle() { return rectangle; };
    SDL_Color& GetColor() { return color; };


    void SetPowerup(Powerup* powerup_p) { powerup = powerup_p; };
    Powerup* GetPowerup() { return powerup; };
    const SDL_Point& GetId() { return id; };

    void Render(SDL_Renderer* renderer);

    void Kill();
    bool IsAlive() { return alive; };
    void Reset();
    uint32_t GetPoints() { return points; };
};