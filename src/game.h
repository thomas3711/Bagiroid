#pragma once
#include "scene.h"

#define PLAYER_START_LIVES 3;

struct Player
{
    int lives = PLAYER_START_LIVES;
    double score = 0.0;
};

class Game
{
private:
    static Game* instance;
    
    Scene* scene;

    Player player;

    // --- Singleton
    // Private constructor to prevent instantiation
    Game();
    
    // Delete copy constructor and assignment operator
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    // ---

    // Brick field
    const SDL_Point brick_dimensions{.x = 150, .y = 32};
    const SDL_Point brick_spacing{.x = 12, .y = 12};
    const SDL_Point brick_count{.x = 8, .y = 6};

    void createBricks();

    // Game window
    const SDL_Color background_color { .r = 30, .g = 30, .b = 30, .a = 255};

public:
    static Game* GetInstance();
    Scene* GetScene() { return scene; };

    ~Game();

    void Initialize();
    void Update(float delta_time);
    void Render(SDL_Renderer* renderer);

    // TODO: rework to some unified event/message system
    void NotifyBallDestruction(Ball* ball);
    void NotifyBrickDestruction(Brick* brick);

    // TODO: maybe move to some window class, or some global static settings
    const int window_width = 2120;
    const int window_height = 1080;

    SDL_Rect gameViewport = {0, 0, 1920, 1080};
    SDL_Rect hudViewport  = {1920, 0, 200, 1080};
};