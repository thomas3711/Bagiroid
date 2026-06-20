#include "game.h"
#include "brick.h"

Game* Game::instance = nullptr;

Game::Game()
{    
}

Game::~Game()
{
}

Game* Game::GetInstance()
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
}

void Game::Update(float delta_time)
{
    scene->Update(delta_time);

    const bool *keys = SDL_GetKeyboardState(nullptr);

    // R - to reset game, when player is dead
    if (player.lives == 0 and keys[SDL_SCANCODE_R])
    {
        SDL_Log("Restarting game !");
        player.lives = PLAYER_START_LIVES;
        scene->GetPaddle().SetControlState(true);
        scene->GetPaddle().GiveBall();
        scene->RespawnAllBricks();
    }
}
void Game::Render(SDL_Renderer* renderer)
{
    SDL_SetRenderViewport(renderer, NULL);
    SDL_SetRenderDrawColor(renderer, background_color.r, background_color.g, background_color.b, background_color.a);
    SDL_RenderClear(renderer);

    // Game viewport
    SDL_SetRenderViewport(renderer, &gameViewport);
    scene->Render(renderer);

    // HUD viewport
    SDL_SetRenderViewport(renderer, &hudViewport);
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
    SDL_FRect hud_background = {0.0f, 0.0f, (float)hudViewport.w, (float)hudViewport.h};
    SDL_RenderFillRect(renderer, &hud_background);

    // Present render
    SDL_SetRenderViewport(renderer, NULL);
    SDL_RenderPresent(renderer);
}

void Game::createBricks()
{    
    SDL_FPoint offset;

    float all_bricks_width = brick_count.x * brick_dimensions.x + (brick_count.x - 1) * brick_spacing.x;

    offset.x = gameViewport.w / 2.0f - all_bricks_width / 2.0f;
    offset.y = 64;

    for (int y = 0; y < brick_count.y; y++)
    {
        for (int x = 0; x < brick_count.x; x++)
        {
            SDL_FPoint position;
            SDL_FPoint dimensions {.x = (float)brick_dimensions.x, .y = (float)brick_dimensions.y};
            SDL_Color color;

            position.x = x * (dimensions.x + brick_spacing.x) + offset.x;
            position.y = y * (dimensions.y + brick_spacing.y) + offset.y;
            
            // Set color (varies by row for visual effect)
            switch (y)
            {
                case 0: color = {255, 0, 0, 255}; break;     // Red
                case 1: color = {255, 127, 0, 255}; break;   // Orange
                case 2: color = {255, 255, 0, 255}; break;   // Yellow
                case 3: color = {0, 255, 0, 255}; break;     // Green
                case 4: color = {0, 0, 255, 255}; break;     // Blue
                case 5: color = {128, 0, 128, 255}; break;   // Purple
                default: color = {255, 255, 255, 255}; break;
            }

            Brick* brick = new Brick(position, dimensions, color);
            scene->AddBrick(brick);
        }
    }
}

void Game::NotifyBrickDestruction(Brick* brick)
{

}

void Game::NotifyBallDestruction(Ball* ball)
{
    if(player.lives == 0)
    {
        SDL_Log("You are dead !");
        scene->GetPaddle().SetControlState(false);
    }
    else if(player.lives > 0)
    {
        player.lives--;
        SDL_Log("Lives remaining: %d", player.lives);
        scene->GetPaddle().GiveBall();
    }
    else {
        SDL_Log("Error: Player has negative lives !");
    }

    scene->RemoveBall(ball);
}