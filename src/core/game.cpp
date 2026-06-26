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

void Game::Initialize(bool load_plugins)
{
    scene = new Scene();
    ui = new UI();
    level_generator = new LevelGenerator();

    if (load_plugins)
    {
        plugins_loaded = level_generator->LoadPlugin();
    }

    scene->GetPaddle().GiveBall();

    generateLevel();

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

    ui->RenderInfoPanelUI(renderer, infoViewport, player, running, plugins_loaded);

    SDL_SetRenderScale(renderer, 1.0f, 1.0f);
    SDL_SetRenderViewport(renderer, NULL);
    SDL_RenderPresent(renderer);
}

void Game::NotifyBrickDestruction(Brick *brick)
{
    player.score += player.score_multiplier * brick->GetPoints();

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
    player.score_multiplier++;

    scene->DeleteAllOfType<Ball>();
    scene->DeleteAllOfType<Brick>();
    scene->GetPaddle().ClearHeldBall();
    scene->GetPaddle().GiveBall();
    generateLevel();
}

void Game::finalizePlayerDeath()
{
    scene->GetPaddle().SetControlState(false);
    scene->DeleteAll();
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
    generateLevel();
    Ball::ResetRadius();
    running = true;
}

void Game::generateLevel()
{
    level_generator->GenerateBricksData(gameViewport.w, gameViewport.h);

    scene->AddBricks(level_generator->data);

    level_generator->FreeBricksData();
}