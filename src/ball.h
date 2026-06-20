#pragma once
#include <SDL3/SDL.h>

class Ball
{
    float speed = 600.0f;
    
    int radius = default_radius;
    bool alive = false;

    SDL_FPoint position;
    SDL_FPoint velocity;

    SDL_FPoint checkScreenEdgeCollision();
    bool isCollidingWithRect(const SDL_FRect& rect);
    void bounce(SDL_FPoint& normal);
    void bounceOffPaddle(const SDL_FRect& paddle);
    SDL_FPoint getRectCollisionNormal(const SDL_FRect& rect);

    const int default_radius = 12;

public:

    Ball();
    Ball(int new_radius);
    ~Ball();

    void Launch(SDL_FPoint& direction);
    void SetPosition(SDL_FPoint& target_position);

    void Render(SDL_Renderer* renderer);
    void Update(float delta_time);

    int GetRadius() { return radius; };
};