#pragma once
#include "object.h"

class Ball : public Object
{
    float speed = 800.0f;
    bool reduce_lives_after_death = true;
    // All balls will have the same radius allways
    static const int default_radius = 8;
    static const int radius_increase = 2;
    static const int max_radius = 128;
    static int radius;

    SDL_FPoint position { 0.0f, 0.0f };
    SDL_FPoint velocity { 0.0f, 0.0f };

    SDL_FPoint checkScreenEdgeCollision();
    void bounce(SDL_FPoint& normal);
    void bounceOffPaddle(const SDL_FRect& paddle);
    SDL_FPoint getRectCollisionNormal(const SDL_FRect& rect);

public:

    Ball();
    Ball(bool reduce_lives_after_death_p);
    ~Ball() override;

    void Launch(SDL_FPoint& direction);
    void SetPosition(SDL_FPoint& target_position);

    void Render(SDL_Renderer* renderer) override;
    void Update(float delta_time) override;
    void Destroy() override;

    int GetRadius() { return radius; };
    bool GetReduceLives() { return reduce_lives_after_death; };

    void static ResetRadius() { radius = default_radius; }
    void static IncreaseRadius();
};