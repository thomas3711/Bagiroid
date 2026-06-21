#include "game.h"
#include "brick.h"

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
    scene->Update(delta_time);

    const bool *keys = SDL_GetKeyboardState(nullptr);

    // R - to reset game, when player is dead
    if (!running and keys[SDL_SCANCODE_R])
    {
        player.lives = PLAYER_START_LIVES;
        player.score = 0;
        player.level = 1;
        scene->GetPaddle().SetControlState(true);
        scene->GetPaddle().GiveBall();
        scene->RespawnAllBricks();
        running = true;
    }
}
void Game::Render(SDL_Renderer *renderer)
{
    SDL_SetRenderViewport(renderer, NULL);
    SDL_SetRenderDrawColor(renderer, game_background_color.r, game_background_color.g, game_background_color.b, game_background_color.a);
    SDL_RenderClear(renderer);

    // Game viewport
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

    // HUD viewport (reset scale: the !running branch above leaves it at 2.0,
    // which would push this viewport off-screen)
    SDL_SetRenderScale(renderer, 1.0f, 1.0f);
    SDL_SetRenderViewport(renderer, &hudViewport);
    SDL_SetRenderDrawColor(renderer, hud_background_color.r, hud_background_color.g, hud_background_color.b, hud_background_color.a);
    SDL_FRect hud_background = {0.0f, 0.0f, (float)hudViewport.w, (float)hudViewport.h};
    SDL_RenderFillRect(renderer, &hud_background);

    // Scale up the debug font. NOTE: render scale also scales the viewport
    // offset/cliprect, so the viewport must be re-set in scaled units or it
    // gets pushed off-screen.
    const float hud_render_scale = 2.0f;
    SDL_SetRenderScale(renderer, hud_render_scale, hud_render_scale);

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
    }
    else
    {
        SDL_RenderDebugTextFormat(renderer, 10.0f, text_y_start, "You are dead !");
    }

    

    // Present render
    SDL_SetRenderScale(renderer, 1.0f, 1.0f);
    SDL_SetRenderViewport(renderer, NULL);
    SDL_RenderPresent(renderer);
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

            Brick *brick = new Brick(position, dimensions, color, points);
            scene->AddBrick(brick);
        }
    }
}

void Game::NotifyBrickDestruction(Brick *brick)
{
    player.score += player.level * brick->GetPoints();

    if(scene->GetAliveBricksCount() <= 0)
    {
        nextLevel();
    }
}

void Game::NotifyBallDestruction(Ball *ball)
{
    Game::GetInstance()->GetScene()->RemoveBall(ball);
    delete ball;

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

void Game::nextLevel()
{
    player.level++;

    scene->RemoveAllBalls();
    scene->GetPaddle().GiveBall();
    scene->RespawnAllBricks();
}

void Game::playerDied()
{
    scene->GetPaddle().SetControlState(false);
    scene->RemoveAllBalls();
    running = false;
}