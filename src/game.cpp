#include "game.h"
#include "brick.h"
#include "draw.h"
#include <vector>
#include <string>

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

    if(player_died_pending)
    {
        finalizePlayerDeath();
        player_died_pending = false;
        next_level_pending = false;
    }
    else if(next_level_pending)
    {
        nextLevel();
        next_level_pending = false;
    }

    const bool *keys = SDL_GetKeyboardState(nullptr);

    // R - to reset game, when player is dead
    if (!running and keys[SDL_SCANCODE_R])
    {
        restartGame();
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
    float text_x_Start = 10.0f;

    if(running)
    {
        // Stats
        SDL_RenderDebugTextFormat(renderer, text_x_Start, text_y_start, "Level: %d", player.level);
        text_y_start += row_spacing;
        SDL_RenderDebugTextFormat(renderer, text_x_Start, text_y_start, "Score: %d", player.score);
        text_y_start += row_spacing;
        SDL_RenderDebugTextFormat(renderer, text_x_Start, text_y_start, "Lives: %d", player.lives);
        text_y_start += row_spacing;
        SDL_RenderDebugTextFormat(renderer, text_x_Start, text_y_start, "Score Multiplier: %dx", player.score_multiplier);

        // Help
        float hint_row_spacing = 24.0f;
        int hint_circle_radius = 8;
        text_y_start = scaledViewport.h - 10.0f - hint_row_spacing * Powerup::Type::COUNT;

        for (int i = 0; i < static_cast<int>(Powerup::Type::COUNT); i++)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            std::string description = "- " + Powerup::GetDescription((Powerup::Type)i);
            SDL_RenderDebugTextFormat(renderer, text_x_Start + hint_circle_radius * 2, text_y_start, description.c_str());
            SDL_Color color = Brick::GetBrickColor(i);
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            Draw::drawFilledCircle(renderer, text_x_Start, text_y_start + hint_circle_radius / 2, hint_circle_radius);
            text_y_start += hint_row_spacing;
        }
    }
    else
    {
        SDL_RenderDebugTextFormat(renderer, text_x_Start, text_y_start, "You are dead !");
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

            color = Brick::GetBrickColor(y);
            points = Brick::GetBrickPoints(y);

            SDL_Point brick_id { .x = x, .y = y };
            Brick *brick = new Brick(position, dimensions, color, points, brick_id);
            
            scene->AddObject(brick);
        }
    }

    scene->ResetAllBricks();
    generatePowerups();
}

void Game::generatePowerups()
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

    if(scene->GetActiveBricksCount() <= 0)
    {
        next_level_pending = true;
    }
}

void Game::NotifyBallDestruction(Ball *ball)
{
    if(ball->GetReduceLives())
    {
        if (player.lives == 0)
        {
            player_died_pending = true;
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
}

void Game::nextLevel()
{
    player.level++;

    scene->DeleteAllOfType<Ball>();
    scene->GetPaddle().ClearHeldBall();
    scene->GetPaddle().GiveBall();
    scene->ResetAllBricks();
    generatePowerups();
}

void Game::finalizePlayerDeath()
{
    scene->GetPaddle().SetControlState(false);
    scene->DeleteAllOfType<Ball>();
    scene->DeleteAllOfType<Powerup>();
    scene->GetPaddle().ClearHeldBall();
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
    scene->GetPaddle().ClearHeldBall();
    scene->GetPaddle().GiveBall();
    scene->ResetAllBricks();
    generatePowerups();
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