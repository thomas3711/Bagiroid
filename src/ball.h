#pragma once
#include <SDL3/SDL.h>

class Ball
{
    float speed = 800.0f;
    bool reduce_lives_after_death = true;
    // All balls will have the same radius allways
    static const int default_radius = 8;
    static const int radius_increase = 4;
    static const int max_radius = 128;
    static int radius;

    SDL_FPoint position;
    SDL_FPoint velocity;

    SDL_FPoint checkScreenEdgeCollision();
    void bounce(SDL_FPoint& normal);
    void bounceOffPaddle(const SDL_FRect& paddle);
    SDL_FPoint getRectCollisionNormal(const SDL_FRect& rect);


public:

    Ball();
    Ball(bool reduce_lives_after_death_p);
    ~Ball();

    void Launch(SDL_FPoint& direction);
    void SetPosition(SDL_FPoint& target_position);

    void Render(SDL_Renderer* renderer);
    void Update(float delta_time);

    int GetRadius() { return radius; };
    bool GetReduceLives() { return reduce_lives_after_death; };

    void static ResetRadius() { radius = default_radius; }
    void static IncreaseRadius();
};