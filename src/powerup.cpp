#include "powerup.h"
#include "draw.h"
#include "game.h"
#include "physics.h"

Powerup::Powerup(SDL_Color& color_p, Type type_p)
{
    radius = default_radius;
    color = color_p;
    type = type_p;
    active = false;
    
    speed = default_speed + (int(Type::COUNT) - int(type)) * speed_increment;
}

Powerup::~Powerup()
{

}

void Powerup::Spawn(SDL_FPoint& position_p)
{
    SetActive(true);
    position = position_p;
    Game::GetInstance()->GetScene()->AddObject(this);

    // This type of power-up doesn't need to be picked up, just kill it and trigger effect
    if(type == Type::spawnBalls)
    {
        triggerEffect();
        Destroy();
    }
}

void Powerup::Render(SDL_Renderer* renderer)
{
    if(!active)
    {
        return;
    }

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    Draw::drawFilledCircle(renderer, position.x, position.y, radius);
}

void Powerup::Update(float delta_time)
{
    if(!active)
    {
        return;
    }

    // Just fall down
    position.y += speed * delta_time;

    // Check collision and against paddle
    SDL_FRect paddle = Game::GetInstance()->GetScene()->GetPaddle().GetRectangle();

    if(Physics::isSphereCollidingWithRect(position, radius, paddle))
    {
        // Find closest point and push ball outside
        float closest_y = SDL_clamp(position.y, paddle.y, paddle.y + paddle.h);
        
        position.y = closest_y - radius;

        triggerEffect();
        Destroy();
    }
    else if(position.y > Game::GetInstance()->window_height + radius)
    {
        Destroy();
    }
}

void Powerup::Destroy()
{
    active = false;
    Game::GetInstance()->GetScene()->DestroyObject(this);
}

std::string Powerup::GetDescription(Type type)
{
    switch(type)
    {
        case Type::biggerBall:
            return "Bigger Ball";
        case Type::fasterPaddle:
            return "Faster Paddle";
        case Type::biggerPaddle:
            return "Bigger Paddle";
        case Type::spawnBalls:
            return "Spawn Balls";
        case Type::life:
            return "Extra Life";
        case Type::scoreMultiplier:
            return "Score Multiplier";
        default:
            return "None";
    }
}

void Powerup::triggerEffect()
{
    switch(type)
    {
        case Type::biggerBall:
            Ball::IncreaseRadius();
            break;
        case Type::fasterPaddle:
            Game::GetInstance()->GetScene()->GetPaddle().IncreaseSpeed();
            break;
        case Type::biggerPaddle:
            Game::GetInstance()->GetScene()->GetPaddle().IncreaseWidth();
            break;
        case Type::spawnBalls:
            Game::GetInstance()->SpawnBalls(position);
            break;
        case Type::life:
            Game::GetInstance()->IncreaseLives();
            break;
        case Type::scoreMultiplier:
            Game::GetInstance()->IncreaseScoreMultiplier();
            break;
        default:
            break;
    }
}