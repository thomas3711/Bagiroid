#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include "ball.h"
#include "paddle.h"
#include "brick.h"

class Scene
{
private:
    std::vector<Ball*> balls;
    std::vector<Brick*> bricks;
    Paddle paddle = Paddle();

    bool debug_print = true;

public:
    Scene();

    std::vector<Brick*>& GetBricks() { return bricks; };

    void Update(float delta_time);
    void Render(SDL_Renderer* renderer);

    void AddBall(Ball* ball_to_add);
    void RemoveBall(Ball* ball_to_remove);

    void AddBrick(Brick* brick_to_add);
    void RespawnAllBricks();

    Paddle& GetPaddle() { return paddle; };
};