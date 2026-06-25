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
    ui = new UI();
    level_generator = new LevelGenerator();

    scene->GetPaddle().GiveBall();

    level_generator->createBricks(scene, gameViewport);
    level_generator->generatePowerups(scene);

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

    if(running)
    {
        SDL_SetRenderViewport(renderer, &gameViewport);
        scene->Render(renderer);
    }
    else
    {
        ui->RenderRestartGameUI(renderer, gameViewport);
    }

    ui->RenderInfoPanelUI(renderer, infoViewport, player, running);

    SDL_SetRenderScale(renderer, 1.0f, 1.0f);
    SDL_SetRenderViewport(renderer, NULL);
    SDL_RenderPresent(renderer);
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
    level_generator->generatePowerups(scene);
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
    level_generator->generatePowerups(scene);
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