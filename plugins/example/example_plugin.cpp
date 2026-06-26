#include "plugin_api.h"

#include <cstdlib>
#include <cmath>
#include <vector>

namespace
{
    constexpr int columns = 30;
    constexpr int rows = 6;
    constexpr float brick_width = 26.0f;
    constexpr float brick_height = 26.0f;
    constexpr float spacing_x = 10.0f;
    constexpr float spacing_y = 12.0f;
    constexpr float top_offset = 96.0f;
}

PLUGIN_EXPORT int plugin_api_version(void)
{
    return PLUGIN_VERSION;
}

PLUGIN_EXPORT GeneratedBrickskData* plugin_generate_bricks(const BrickGenContext* ctx)
{
    const int count = columns * rows;

    auto* result = static_cast<GeneratedBrickskData*>(std::malloc(sizeof(GeneratedBrickskData)));
    if (result == nullptr)
    {
        return nullptr;
    }

    result->count = count;
    result->bricks = static_cast<BrickData*>(std::calloc(count, sizeof(BrickData)));
    if (result->bricks == nullptr)
    {
        std::free(result);
        return nullptr;
    }

    const float all_bricks_width = columns * brick_width + (columns - 1) * spacing_x;
    const float offset_x = ctx->game_width / 2.0f - all_bricks_width / 2.0f;

    int index = 0;

    for (int y = 0; y < rows; y++)
    {
        for (int x = 0; x < columns; x++)
        {
            BrickData& brick = result->bricks[index];

            brick.w = brick_width;
            brick.h = brick_height;
            brick.x = x * (brick_width + spacing_x) + offset_x;
            brick.y = y * (brick_height + spacing_y) + top_offset;

            brick.points = 1000;

            auto roll = std::rand() % 3;

            if(roll == 0)
            {
                brick.r = 255;
                brick.g = 255;
                brick.b = 0;
                brick.powerup = 2;
            }
            else if (roll == 1)
            {
                brick.r = 0;
                brick.g = 0;
                brick.b = 255;
                brick.powerup = 4;
            }
            else
            {
                brick.r = 0;
                brick.g = 255;
                brick.b = 0;
                brick.powerup = 3;
            }

            index++;
        }
    }

    return result;
}

PLUGIN_EXPORT void plugin_free_bricks(GeneratedBrickskData* data)
{
    if (data == nullptr)
    {
        return;
    }

    std::free(data->bricks);
    std::free(data);
}
