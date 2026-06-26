#pragma once
#include "scene.h"
#include "ui.h"
#include "level_generator.h"
#include "player.h"

// Master class that handles initialization, game state flow and 
// everything that does not yet has its own place
class Game
{
private:
    static Game* instance;
    
    Scene* scene;
    UI* ui;
    LevelGenerator *level_generator;

    Player player;

    bool running = false;

    bool next_level_pending = false;
    bool player_died_pending = false;

    bool plugins_loaded = false;

    // --- Singleton
    // Private constructor to prevent instantiation
    Game();
    
    // Delete copy constructor and assignment operator
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    // ---

    // Game window
    const SDL_Color game_background_color { .r = 30, .g = 30, .b = 30, .a = 255};

    // Game
    void nextLevel();
    void finalizePlayerDeath();
    void restartGame();
    void generateLevel();

public:
    static Game* GetInstance();
    Scene* GetScene() { return scene; };

    ~Game();

    void Initialize(bool load_plugins = false);
    void Update(float delta_time);
    void Render(SDL_Renderer* renderer);

    void IncreaseScoreMultiplier() { player.score_multiplier += 1; };
    void IncreaseLives() { player.lives++; };

    // TODO: rework to some unified event/message system
    void NotifyBallDestruction(Ball* ball);
    void NotifyBrickDestruction(Brick* brick);

    // TODO: maybe move to some window class, or some global static settings
    const SDL_Rect gameViewport = {0, 0, 1520, 1080};
    const SDL_Rect infoViewport  = {gameViewport.w, 0, 400, 1080};

    const int window_width = gameViewport.w + infoViewport.w;
    const int window_height = 1080;

    
};