#pragma once
#include "scene.h"
#include "ui.h"
#include "level_generator.h"
#include "player.h"

#define APPLICATION_NAME "Bagiroid"

// Master class that handles initialization, game state flow and 
// everything that does not yet has its own place
class Game
{
private:
    // --- Singleton    
    static Game* instance;    
    Game();
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    // ---
    
    SDL_Renderer* renderer;
    SDL_Window* window;
    Scene* scene;
    UI* ui;
    LevelGenerator *level_generator;

    Player player;

    bool game_running = false;
    bool application_running = false;

    bool next_level_pending = false;
    bool player_died_pending = false;

    bool plugins_loaded = false;

    Uint64 last_frame_time;

    SDL_Event event;

    // Game
    void nextLevel();
    void finalizePlayerDeath();
    void restartGame();
    void generateLevel();

public:
    static Game* GetInstance();
    Scene* GetScene() { return scene; };

    ~Game();

    void Initialize(int argc, char* argv[]);
    void Quit();
    void Update();
    void Render();

    void IncreaseScoreMultiplier() { player.score_multiplier += 1; };
    void IncreaseLives() { player.lives++; };

    bool IsApplicationRunning() { return application_running; };

    void NotifyBallDestruction(Ball* ball);
    void NotifyBrickDestruction(Brick* brick);

    const SDL_Rect game_viewport = {0, 0, 1520, 1080};
    const SDL_Rect infoViewport  = {game_viewport.w, 0, 400, 1080};

    const int window_width = game_viewport.w + infoViewport.w;
    const int window_height = 1080;

    
};