#pragma once
#include <SDL3/SDL.h>
#include "ball.h"

// Player controlled paddle at the bottom of the screen.
// Handles input, control, update and render of the paddle
class Paddle
{
    const int default_width = 150;
    const int height = 10;
    const float width_increase = 25;    
    const float default_speed = 750.0f;
    const float speed_increase = 50.0f;
    const float max_speed = 2000.0f;
    const int color = 192;

    float speed = 750.0f;

    int x_min_pos;
    int x_max_pos;

    SDL_FRect rectangle;
    SDL_Point game_viewport;
    SDL_FPoint ball_position;

    bool control_enabled = true;

    Ball* ball = nullptr;
public:
    Paddle();

    void Render(SDL_Renderer* renderer);
    void Update(float delta_time);

    void ResetToDefault();
    void IncreaseWidth();
    void IncreaseSpeed();

    float GetPositionX() { return rectangle.x; };
    float GetPositionY() { return rectangle.y; };

    void GiveBall();
    void ClearHeldBall() { ball = nullptr; };
    SDL_FRect GetRectangle() { return rectangle; };

    void SetControlState(bool state) { control_enabled = state; };

private:
    void setStartPosition();
    void updateBallPosition();
};