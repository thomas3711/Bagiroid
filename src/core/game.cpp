#include "game.h"
#include "brick.h"
#include "draw.h"
#include <vector>
#include <string>

Game::~Game()
{
}

Game *Game::GetInstance()
{
    static Game instance;
    return &instance;
}

void Game::Initialize(int argc, char* argv[])
{
    // Initialize main objects
    window = SDL_CreateWindow(APPLICATION_NAME, window_width, window_height, SDL_WINDOW_FULLSCREEN);
    renderer = SDL_CreateRenderer(window, nullptr);
    SDL_SetRenderLogicalPresentation(renderer, window_width, window_height, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    // Enable vsync so the main loop doesn't busy-wait
    SDL_SetRenderVSync(renderer, 1);

    scene = std::make_unique<Scene>();
    ui = std::make_unique<UI>();
    level_generator = std::make_unique<LevelGenerator>();

    // Check program arguments and load plugins if necessary
    bool load_plugins = false;
    for (int i = 1; i < argc; i++)
    {
        if (SDL_strcmp(argv[i], "-p") == 0 || SDL_strcmp(argv[i], "--plugins") == 0)
        {
            load_plugins = true;
        }
    }

    if (load_plugins)
    {
        plugins_loaded = level_generator->LoadPlugin();
    }

    // Setup the game stage
    scene->GetPaddle().GiveBall();
    generateLevel();
    
    // Start delta time calculation
    last_frame_time = SDL_GetTicks();

    // State
    application_running = true;
    game_running = true;
}

void Game::Quit()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::Update()
{
    const bool *keys = SDL_GetKeyboardState(nullptr);

    // Calculate delta time in seconds
    Uint64 current_frame_time = SDL_GetTicks();
    float delta_time = (current_frame_time - last_frame_time) / 1000.0f;
    last_frame_time = current_frame_time;

    // Drain the event queue, then check quit conditions separately
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            application_running = false;
        }
    }

    if (keys[SDL_SCANCODE_ESCAPE])
    {
        application_running = false;
    }

    // Update scene
    if(game_running)
    {
        scene->Update(delta_time);
    }

    // Check state changes
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

    // Check R press to reset game, when player is dead
    if (!game_running && keys[SDL_SCANCODE_R])
    {
        restartGame();
    }
}

void Game::Render()
{
    // Clear
    SDL_SetRenderViewport(renderer, NULL);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Game viewport render
    SDL_SetRenderViewport(renderer, &game_viewport);
    ui->RenderGameBackground(renderer, game_viewport);

    if(game_running)
    {
        scene->Render(renderer);
    }
    else
    {
        ui->RenderRestartGameUI(renderer, game_viewport);
    }

    // Info panel render
    ui->RenderInfoPanelUI(renderer, info_viewport, player, game_running, plugins_loaded);

    // Reset and present
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
    game_running = false;
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
    game_running = true;
}

void Game::generateLevel()
{
    level_generator->GenerateBricksData(game_viewport.w, game_viewport.h);

    scene->AddBricks(level_generator->GetData());

    level_generator->FreeBricksData();
}