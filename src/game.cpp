#include "game.h"
#include "brick.h"
#include <vector>

Game *Game::instance = nullptr;

Game::Game()
{
}

Game::~Game()
{
}

Game *Game::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new Game();
    }

    return instance;
}

void Game::Initialize()
{
    scene = new Scene();

    scene->GetPaddle().GiveBall();

    createBricks();

    running = true;
}

void Game::Update(float delta_time)
{
    if(running)
    {
        scene->Update(delta_time);
    }

    const bool *keys = SDL_GetKeyboardState(nullptr);

    // R - to reset game, when player is dead
    if (!running and keys[SDL_SCANCODE_R])
    {
        player.lives = PLAYER_START_LIVES;
        player.score = 0;
        player.level = 1;
        scene->GetPaddle().SetControlState(true);
        scene->GetPaddle().GiveBall();
        scene->ResetAllBricks();
        running = true;
    }
}
void Game::Render(SDL_Renderer *renderer)
{
    SDL_SetRenderViewport(renderer, NULL);
    SDL_SetRenderDrawColor(renderer, game_background_color.r, game_background_color.g, game_background_color.b, game_background_color.a);
    SDL_RenderClear(renderer);

    renderGameViewport(renderer);

    renderHUDViewport(renderer);

    SDL_SetRenderScale(renderer, 1.0f, 1.0f);
    SDL_SetRenderViewport(renderer, NULL);
    SDL_RenderPresent(renderer);
}

void Game::renderGameViewport(SDL_Renderer* renderer)
{
    if(running)
    {
        SDL_SetRenderViewport(renderer, &gameViewport);
        SDL_SetRenderScale(renderer, 1.0f, 1.0f);

        scene->Render(renderer);
    }
    else
    {
        const float game_render_scale = 2.0f;
        SDL_SetRenderScale(renderer, game_render_scale, game_render_scale);

        SDL_Rect scaledViewport = {
            (int)(gameViewport.x / game_render_scale),
            (int)(gameViewport.y / game_render_scale),
            (int)(gameViewport.w / game_render_scale),
            (int)(gameViewport.h / game_render_scale)
        };

        SDL_SetRenderViewport(renderer, &scaledViewport);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        const char *msg = "Press [R] to restart the game.";
        float msg_width = SDL_strlen(msg) * SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE;

        float center_x = gameViewport.w / game_render_scale / 2.0f;
        float center_y = gameViewport.h / game_render_scale / 2.0f;

        SDL_RenderDebugText(renderer,
            center_x - msg_width / 2.0f,
            center_y - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE / 2.0f,
            msg
        );
    }
}

void Game::renderHUDViewport(SDL_Renderer* renderer)
{
    SDL_SetRenderScale(renderer, 1.0f, 1.0f);
    SDL_SetRenderViewport(renderer, &hudViewport);

    SDL_SetRenderDrawColor(renderer, hud_background_color.r, hud_background_color.g, hud_background_color.b, hud_background_color.a);
    SDL_FRect hud_background = {0.0f, 0.0f, (float)hudViewport.w, (float)hudViewport.h};
    SDL_RenderFillRect(renderer, &hud_background);

    const float hud_render_scale = 2.0f;
    SDL_SetRenderScale(renderer, hud_render_scale, hud_render_scale);

    // Render scale, scales the viewport as well
    SDL_Rect scaledViewport = {
        (int)(hudViewport.x / hud_render_scale),
        (int)(hudViewport.y / hud_render_scale),
        (int)(hudViewport.w / hud_render_scale),
        (int)(hudViewport.h / hud_render_scale)
    };

    SDL_SetRenderViewport(renderer, &scaledViewport);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    const float row_spacing = 16.0f;
    float text_y_start = 10.0f;

    if(running)
    {
        SDL_RenderDebugTextFormat(renderer, 10.0f, text_y_start, "Level: %d", player.level);
        text_y_start += row_spacing;
        SDL_RenderDebugTextFormat(renderer, 10.0f, text_y_start, "Score: %d", player.score);
        text_y_start += row_spacing;
        SDL_RenderDebugTextFormat(renderer, 10.0f, text_y_start, "Lives: %d", player.lives);
        text_y_start += row_spacing;
        SDL_RenderDebugTextFormat(renderer, 10.0f, text_y_start, "Score Multiplier: %dx", player.score_multiplier);
    }
    else
    {
        SDL_RenderDebugTextFormat(renderer, 10.0f, text_y_start, "You are dead !");
    }
}

// TODO: maybe move to scene ?
void Game::createBricks()
{
    SDL_FPoint offset;

    float all_bricks_width = brick_count.x * brick_dimensions.x + (brick_count.x - 1) * brick_spacing.x;

    offset.x = gameViewport.w / 2.0f - all_bricks_width / 2.0f;
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

            // Set color (varies by row for visual effect)
            switch (y)
            {
            case 0:
                color = {255, 0, 0, 255};
                points = 32;
                break; // Red
            case 1:
                color = {255, 127, 0, 255};
                points = 16;
                break; // Orange
            case 2:
                color = {255, 255, 0, 255};
                points = 8;
                break; // Yellow
            case 3:
                color = {0, 255, 0, 255};
                points = 4;
                break; // Green
            case 4:
                color = {0, 0, 255, 255};
                points = 2;
                break; // Blue
            case 5:
                color = {128, 0, 128, 255};
                points = 1;
                break; // Purple
            default:
                color = {255, 255, 255, 255};
                points = 0;
                break;
            }

            SDL_Point brick_id { .x = x, .y = y };
            Brick *brick = new Brick(position, dimensions, color, points, brick_id);
            
            scene->AddBrick(brick);
        }
    }

    addPowerupsToBricks();
}

void Game::addPowerupsToBricks()
{
    for (int y = 0; y < brick_count.y; y++)
    {
        // For each row pick X distinct random columns that will hold a powerup.
        int bricks_with_powerup = 8;
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
                //Powerup* powerup = new Powerup(brick->GetColor(), (Powerup::Type)(brick_count.y - y - 1));
                Powerup* powerup = new Powerup(brick->GetColor(), Powerup::Type::spawnBalls);
                
                brick->SetPowerup(powerup);
            }
        }
    }
}

void Game::NotifyBrickDestruction(Brick *brick)
{
    player.score += player.level * player.score_multiplier * brick->GetPoints();

    Powerup* powerup = brick->GetPowerup();

    if(powerup != nullptr)
    {
        SDL_FRect brick_rect = brick->GetRectangle();
        SDL_FPoint position { .x = brick_rect.x + brick_rect.w / 2.0f, .y = brick_rect.y + brick_rect.h / 2.0f };
        powerup->Spawn(position);
        brick->SetPowerup(nullptr);
    }

    if(scene->GetAliveBricksCount() <= 0)
    {
        nextLevel();
    }
}

void Game::NotifyBallDestruction(Ball *ball)
{
    if(ball->GetReduceLives())
    {
        if (player.lives == 0)
        {
            playerDied();
        }
        else if (player.lives > 0)
        {
            player.lives--;
            scene->GetPaddle().GiveBall();
        }
        else
        {
            SDL_Log("Error: Player has negative lives !");
        }
    }

    Game::GetInstance()->GetScene()->RemoveBall(ball);
    delete ball;
}

void Game::nextLevel()
{
    player.level++;

    scene->RemoveAllBalls();
    scene->GetPaddle().GiveBall();
    scene->ResetAllBricks();
    addPowerupsToBricks();
}

void Game::playerDied()
{
    scene->GetPaddle().SetControlState(false);
    scene->RemoveAllBalls();
    scene->RemoveAllPowerups();
    running = false;
}

void Game::restartGame()
{
    player.lives = PLAYER_START_LIVES;
    player.score = 0;
    player.level = 1;
    player.score_multiplier = 1;
    scene->GetPaddle().ResetToDefault();
    scene->GetPaddle().SetControlState(true);
    scene->GetPaddle().GiveBall();
    scene->ResetAllBricks();
    addPowerupsToBricks();
    Ball::ResetRadius();
    running = true;
}

void Game::SpawnBalls(SDL_FPoint position)
{
    const int ball_count = 3;
    const float angle_step = (2.0f * SDL_PI_F) / ball_count;

    float base_angle = SDL_randf() * 2.0f * SDL_PI_F;

    for (int i = 0; i < ball_count; i++)
    {
        float angle = base_angle + i * angle_step;

        Ball* ball = new Ball(false);

        SDL_FPoint spawn_position = position;
        ball->SetPosition(spawn_position);

        SDL_FPoint direction;
        direction.x = SDL_cosf(angle);
        direction.y = SDL_sinf(angle);
        ball->Launch(direction);
    }
}