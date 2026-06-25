#include "level_generator.h"

void LevelGenerator::createBricks(Scene* scene, const SDL_Rect& viewport)
{
    SDL_FPoint offset;

    float all_bricks_width = brick_count.x * brick_dimensions.x + (brick_count.x - 1) * brick_spacing.x;

    offset.x = viewport.w / 2.0f - all_bricks_width / 2.0f;
    offset.y = 96;

    for (int y = 0; y < brick_count.y; y++)
    {
        for (int x = 0; x < brick_count.x; x++)
        {
            SDL_FPoint position;
            SDL_FPoint dimensions{.x = (float)brick_dimensions.x, .y = (float)brick_dimensions.y};
            SDL_Color color;
            uint32_t points = 0;

            position.x = x * (dimensions.x + brick_spacing.x) + offset.x;
            position.y = y * (dimensions.y + brick_spacing.y) + offset.y;

            color = Brick::GetBrickColor(y);
            points = Brick::GetBrickPoints(y);

            SDL_Point brick_id { .x = x, .y = y };
            Brick *brick = new Brick(position, dimensions, color, points, brick_id);
            
            scene->AddObject(brick);
        }
    }

    scene->ResetAllBricks();
}

void LevelGenerator::generatePowerups(Scene* scene)
{
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
            SDL_Point brick_id { .x = x, .y = y };
            Brick* brick = scene->GetBrick(brick_id);

            // Add powerup if the brick has one
            if(column_has_powerup[x])
            {
                Powerup* powerup = new Powerup(brick->GetColor(), (Powerup::Type)y);
                //Powerup* powerup = new Powerup(brick->GetColor(), Powerup::Type::spawnBalls);
                
                brick->SetPowerup(powerup);
            }
        }
    }
}
