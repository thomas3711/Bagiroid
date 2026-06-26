#include "draw.h"

namespace Draw
{
    void drawFilledCircle(SDL_Renderer* renderer, int cx, int cy, int radius_p)
    {
        for (int y = -radius_p; y <= radius_p; y++)
        {
            int dx = (int)SDL_sqrt((double)(radius_p * radius_p - y * y));
            SDL_RenderLine(renderer, cx - dx, cy + y, cx + dx, cy + y);
        }
    }
}

