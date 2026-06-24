#pragma once
#include "object.h"

class Powerup : public Object
{
public:
    enum Type
    {
        none = -1,
        biggerBall = 0,
        fasterPaddle,
        biggerPaddle,
        spawnBalls,
        life,
        doublePointModifier
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

public:
    Powerup(SDL_Color& color_p, Type type_p);
    ~Powerup() override;

    void Spawn(SDL_FPoint& position_p);
    void Kill();

    void Render(SDL_Renderer* renderer) override;
    void Update(float delta_time) override;

    Type GetType() { return type; };
};