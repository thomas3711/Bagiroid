#pragma once
#include "scene.h"

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

    // Brick field
    const SDL_Point brick_dimensions{.x = 122, .y = 26};
    const SDL_Point brick_spacing{.x = 12, .y = 12};
    //const SDL_Point brick_count{.x = 1, .y = 6};
    const SDL_Point brick_count{.x = 8, .y = 6};

    void createBricks();
    void generatePowerups();

    // Game window
    const SDL_Color game_background_color { .r = 30, .g = 30, .b = 30, .a = 255};
    const SDL_Color hud_background_color { .r = 45, .g = 45, .b = 45, .a = 255};

    // Game
    void nextLevel();
    void finalizePlayerDeath();
    void restartGame();

    // Render
    void renderGameViewport(SDL_Renderer* renderer);
    void renderHUDViewport(SDL_Renderer* renderer);

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
    const SDL_Rect hudViewport  = {gameViewport.w, 0, 400, 1080};

    const int window_width = gameViewport.w + hudViewport.w;
    const int window_height = 1080;
};