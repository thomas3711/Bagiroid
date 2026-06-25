#pragma once
#include <SDL3/SDL.h>
#include "object.h"
#include "powerup.h"

class Brick : public Object
{
    SDL_FRect rectangle;
    SDL_Color color;
    uint32_t points = 0;
    Powerup* powerup = nullptr;
    SDL_Point id;
public:

    Brick();
    Brick(SDL_FPoint position, SDL_FPoint dimensions, SDL_Color color_p, uint32_t points_p, SDL_Point id_p);

    const SDL_FRect& GetRectangle() { return rectangle; };
    SDL_Color& GetColor() { return color; };

    void SetPowerup(Powerup* powerup_p) { powerup = powerup_p; };
    Powerup* GetPowerup() { return powerup; };
    const SDL_Point& GetId() { return id; };

    void Render(SDL_Renderer* renderer) override;
    void Update(float delta_time) override;

    void Destroy() override;
    void Reset();
    uint32_t GetPoints() { return points; };

    static SDL_Color GetBrickColor(int row);
    static int GetBrickPoints(int row);
};