#pragma once
#include "scene.h"
#include "ui.h"
#include "level_generator.h"

#define PLAYER_START_LIVES 3;

struct Player
{
    int lives = PLAYER_START_LIVES;
    uint64_t score = 0;
    uint16_t level = 1;
    uint64_t score_multiplier = 1;
};

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

public:
    static Game* GetInstance();
    Scene* GetScene() { return scene; };

    ~Game();

    void Initialize();
    void Update(float delta_time);
    void Render(SDL_Renderer* renderer);

    void IncreaseScoreMultiplier() { player.score_multiplier += 1; };
    void IncreaseLives() { player.lives++; };
    void SpawnBalls(SDL_FPoint position);

    // TODO: grid generation should be in its own class - the functionality is piling up in Game

    // TODO: rework to some unified event/message system
    void NotifyBallDestruction(Ball* ball);
    void NotifyBrickDestruction(Brick* brick);

    // TODO: maybe move to some window class, or some global static settings
    const SDL_Rect gameViewport = {0, 0, 1520, 1080};
    const SDL_Rect infoViewport  = {gameViewport.w, 0, 400, 1080};

    const int window_width = gameViewport.w + infoViewport.w;
    const int window_height = 1080;

    
};