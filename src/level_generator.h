#pragma once
#include <SDL3/SDL.h>
#include "scene.h"
#include "plugin_api.h"

class LevelGenerator
{
public:
    const SDL_FPoint brick_dimensions{.x = 122, .y = 26};
    const SDL_FPoint brick_spacing{.x = 12, .y = 12};
    const SDL_FPoint brick_count{.x = 8, .y = 6};

    GeneratedBrickskData* data;

    void GenerateBricksData(const int& game_width, const int& game_height);
    void FreeBricksData();

    
    SDL_Color GetBrickColor(int row);
    int GetBrickPoints(int row);
};
