#pragma once
#include "object.h"

// Ball that player launches from paddle, as well as balls spawned from power up
// Handles movement, rendering and collision
class Ball : public Object
{
    static constexpr int default_radius = 8;
    static constexpr int radius_increase = 1;
    static constexpr int max_radius = 128;

    // All balls will have the same radius for convenience
    static int radius;

    float speed = 1000.0f;
    bool reduce_lives_after_death = true;

    SDL_FPoint position { 0.0f, 0.0f };
    SDL_FPoint velocity { 0.0f, 0.0f };
    SDL_Point game_viewport;

    SDL_FPoint checkScreenEdgeCollision();
    void bounce(const SDL_FPoint& normal);
    void bounceOffPaddle(const SDL_FRect& paddle);
    SDL_FPoint resolveRectCollision(const SDL_FRect& rect);

public:

    Ball(bool reduce_lives_after_death_p = true);
    ~Ball() override;

    void Launch(SDL_FPoint& direction, float speed_factor);
    void SetPosition(SDL_FPoint& target_position);

    void Render(SDL_Renderer* renderer) override;
    void Update(float delta_time) override;
    void Destroy() override;

    int GetRadius() const { return radius; };
    bool GetReduceLives() const { return reduce_lives_after_death; };

    void static ResetRadius() { radius = default_radius; }
    void static IncreaseRadius();
};