#pragma once
#include <memory>
#include "scene.h"
#include "ui.h"
#include "level_generator.h"
#include "player.h"

inline constexpr const char* APPLICATION_NAME = "Bagiroid";

// Master class that handles initialization, game state flow and
// everything that does not yet has its own place
class Game
{
private:
    // --- Singleton
    Game() = default;
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    // ---

    SDL_Renderer* renderer = nullptr;
    SDL_Window* window = nullptr;
    std::unique_ptr<Scene> scene;
    std::unique_ptr<UI> ui;
    std::unique_ptr<LevelGenerator> level_generator;

    Player player;

    bool game_running = false;
    bool application_running = false;

    bool next_level_pending = false;
    bool player_died_pending = false;

    bool plugins_loaded = false;

    Uint64 last_frame_time = 0;

    SDL_Event event;


    // Game
    void nextLevel();
    void finalizePlayerDeath();
    void restartGame();
    void generateLevel();

public:
    static Game* GetInstance();
    Scene* GetScene() const { return scene.get(); };

    ~Game();

    void Initialize(int argc, char* argv[]);
    void Quit();
    void Update();
    void Render();

    void IncreaseScoreMultiplier() { player.score_multiplier += 1; };
    void IncreaseLives() { player.lives++; };

    bool IsApplicationRunning() const { return application_running; };

    void NotifyBallDestruction(Ball* ball);
    void NotifyBrickDestruction(Brick* brick);

    const SDL_Rect game_viewport = {0, 0, 1520, 1080};
    const SDL_Rect info_viewport  = {game_viewport.w, 0, 400, 1080};

private:
    // Fixed 1680x1080
    const int window_width = game_viewport.w + info_viewport.w;
    const int window_height = 1080;
};