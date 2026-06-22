#pragma once
#include <SDL3/SDL.h>

namespace Physics
{
     bool isSphereCollidingWithRect(const SDL_FPoint& sphere_pos, const int& radius, const SDL_FRect& rect);
}
