#include "level_generator.h"

bool LevelGenerator::LoadPlugin(const std::string& directory)
{
    const char* base_path = SDL_GetBasePath();
    std::string resolved = base_path ? std::string(base_path) + directory : directory;

    return plugin.LoadFromDirectory(resolved);
}

void LevelGenerator::GenerateBricksData(const int& game_width, const int& game_height)
{
    if (plugin.IsLoaded())
    {
        BrickGenContext context{ .game_width = game_width, .game_height = game_height };
        data = plugin.Generate(context);

        if (data != nullptr and data->count >= 0 and (data->count == 0 || data->bricks != nullptr))
        {
            data_from_plugin = true;
            return;
        }

        SDL_Log("Plugin '%s' returned no data, falling back to built-in generator", plugin.Path().c_str());
    }

    data_from_plugin = false;
    GenerateBricksDataBuiltin(game_width, game_height);
}

void LevelGenerator::GenerateBricksDataBuiltin(const int& game_width, const int& game_height)
{
    int count = brick_count.x * brick_count.y;

    data = new GeneratedBrickskData();
    data->count = count;
    data->bricks = new BrickData[count]();

    SDL_FPoint offset;

    float all_bricks_width = brick_count.x * brick_dimensions.x + (brick_count.x - 1) * brick_spacing.x;

    offset.x = game_width / 2.0f - all_bricks_width / 2.0f;
    offset.y = 96;

    int index = 0;

    for (int y = 0; y < brick_count.y; y++)
    {
        // For each row pick X distinct random columns that will hold a powerup.
        int bricks_with_powerup = 3;
        bricks_with_powerup = SDL_min(bricks_with_powerup, brick_count.x);

        std::vector<int> pool(brick_count.x);
        for (int i = 0; i < brick_count.x; i++)
        {
            pool[i] = i;
        }

        std::vector<bool> column_has_powerup(brick_count.x, false);
        for (int i = 0; i < bricks_with_powerup; i++)
        {
            int pick = SDL_rand((int)pool.size());
            column_has_powerup[pool[pick]] = true;
            pool.erase(pool.begin() + pick);
        }

        for (int x = 0; x < brick_count.x; x++)
        {
            BrickData& brick_data = data->bricks[index];

            brick_data.w = brick_dimensions.x;
            brick_data.h = brick_dimensions.y;

            brick_data.x = x * (brick_data.w + brick_spacing.x) + offset.x;
            brick_data.y = y * (brick_data.h + brick_spacing.y) + offset.y;

            brick_data.points = LevelGenerator::GetBrickPoints(y);
            SDL_Color color = LevelGenerator::GetBrickColor(y);
            brick_data.r = color.r;
            brick_data.g = color.g;
            brick_data.b = color.b;

            brick_data.powerup = column_has_powerup[x] ? y : -1;

            index++;
        }
    }
}

void LevelGenerator::FreeBricksData()
{
    if(data == nullptr)
    {
        return;
    }

    if(data_from_plugin)
    {
        plugin.Free(data);
    }
    else
    {
        delete[] data->bricks;
        delete data;
    }

    data = nullptr;
    data_from_plugin = false;
}



SDL_Color LevelGenerator::GetBrickColor(int row)
{
    SDL_Color color = { .r = 255, .g = 255, .b = 255, .a = 255 };

    switch (row)
    {
    case 0:
        color = {255, 0, 0, 255};
        break; // Red
    case 1:
        color = {255, 127, 0, 255};
        break; // Orange
    case 2:
        color = {255, 255, 0, 255};
        break; // Yellow
    case 3:
        color = {0, 255, 0, 255};
        break; // Green
    case 4:
        color = {0, 0, 255, 255};
        break; // Blue
    case 5:
        color = {128, 0, 128, 255};
        break; // Purple
    default:
        color = {255, 255, 255, 255};
        break;
    }

    return color;
}

int LevelGenerator::GetBrickPoints(int row)
{
    int exponent = 5 - row;
    int points = SDL_pow(2, exponent);
    return points;
}