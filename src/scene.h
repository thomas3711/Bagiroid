#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include "ball.h"
#include "paddle.h"
#include "brick.h"
#include "powerup.h"

class Scene
{
private:
    std::vector<Ball*> balls;
    std::vector<Powerup*> powerups;
    std::vector<Brick*> bricks;
    Paddle paddle = Paddle();

    bool debug_print = true;

public:
    Scene();

    void Update(float delta_time);
    void Render(SDL_Renderer* renderer);

    void AddBall(Ball* ball_to_add);
    void RemoveBall(Ball* ball_to_remove);
    void RemoveAllBalls();

    void AddPowerup(Powerup* powerup_to_add);
    void RemovePowerup(Powerup* powerup_to_remove);
    void RemoveAllPowerups();

    void AddBrick(Brick* brick_to_add);
    void ResetAllBricks();
    int GetAliveBricksCount();
    Brick* GetBrick(SDL_Point id);
    std::vector<Brick*>& GetBricks() { return bricks; };

    Paddle& GetPaddle() { return paddle; };
};