#pragma once
#include <SDL3/SDL.h>
#include "ball.h"

class Paddle 
{
    const int width = 150;
    const int height = 10;
    const float speed = 600.0f;
    const int color = 192;

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

    float GetPositionX() { return rectangle.x; };
    float GetPositionY() { return rectangle.y; };
    float GetWidth() { return width; };

    void GiveBall();
    const SDL_FRect& GetRectangle() { return rectangle; };

    void SetControlState(bool state) { control_enabled = state; };

private:
    void setStartPosition();
    void updateBallPosition();
};