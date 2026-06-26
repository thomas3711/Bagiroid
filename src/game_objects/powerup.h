#pragma once
#include <string>
#include "object.h"

// Powerup that spawns as a reward from some bricks.
// It applies the effect to the game in triggerEffect();
class Powerup : public Object
{
public:
    enum Type
    {
        none = -1,
        scoreMultiplier = 0,
        life,
        spawnBalls,
        biggerPaddle,
        fasterPaddle,
        biggerBall,
        COUNT
    };

private:
    Type type = Type::none;
    float speed = 250.0f;
    const float default_speed = 250.0f;
    const float speed_increment = 50.0f;
    const int default_radius = 12;
    int radius;

    SDL_FPoint position;
    SDL_Color color;

    void triggerEffect();
    void spawnBallsEffect(SDL_FPoint position);

public:
    Powerup(Type type_p);
    ~Powerup() override;

    void Spawn(SDL_FPoint& position_p);
    void Destroy() override;

    void Render(SDL_Renderer* renderer) override;
    void Update(float delta_time) override;

    Type GetType() { return type; };
    static SDL_Color GetColor(Type type_p);

    static std::string GetDescription(Type type);
};