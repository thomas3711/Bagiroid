#pragma once
#include <SDL3/SDL.h>
#include "object.h"
#include "powerup.h"

class Brick : public Object
{
    SDL_FRect rectangle;
    SDL_Color color;
    uint32_t points = 0;
    SDL_Point id;
    Powerup::Type powerup_type;
public:

    Brick();
    Brick(SDL_FRect rectangle_p, SDL_Color color_p, uint32_t points_p, Powerup::Type powerup_type_p);

    const SDL_FRect& GetRectangle() { return rectangle; };
    SDL_Color& GetColor() { return color; };

    const SDL_Point& GetId() { return id; };

    void Render(SDL_Renderer* renderer) override;
    void Update(float delta_time) override;

    void Destroy() override;
    uint32_t GetPoints() { return points; };
};