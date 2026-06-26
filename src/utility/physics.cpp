#include "physics.h"

namespace Physics
{
    bool isSphereCollidingWithRect(const SDL_FPoint& sphere_pos, const int& radius, const SDL_FRect& rect)
    {
        // Find closest point on rectangle to circle center
        float closest_x = SDL_clamp(sphere_pos.x, rect.x, rect.x + rect.w);
        float closest_y = SDL_clamp(sphere_pos.y, rect.y, rect.y + rect.h);
        
        // Calculate distance between circle center and closest point
        float dx = sphere_pos.x - closest_x;
        float dy = sphere_pos.y - closest_y;
        float distance = SDL_sqrt(dx*dx + dy*dy);
        
        return distance < radius;
    }
}