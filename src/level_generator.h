#pragma once
#include <SDL3/SDL.h>
#include "scene.h"

class LevelGenerator
{
public:
    // Brick field
    const SDL_Point brick_dimensions{.x = 122, .y = 26};
    const SDL_Point brick_spacing{.x = 12, .y = 12};
    const SDL_Point brick_count{.x = 8, .y = 6};

    void createBricks(Scene* scene, const SDL_Rect& viewport);
    void generatePowerups(Scene* scene);
};
