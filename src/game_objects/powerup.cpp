#include "powerup.h"
#include "draw.h"
#include "game.h"
#include "physics.h"

Powerup::Powerup(Type type_p)
{
    type = type_p;
    radius = default_radius;
    active = false;
    color = GetColor(type);
    speed = default_speed + (int(Type::COUNT) - int(type)) * speed_increment;

    game_viewport.x = Game::GetInstance()->game_viewport.w;
    game_viewport.y = Game::GetInstance()->game_viewport.h;
}

Powerup::~Powerup()
{

}

void Powerup::Spawn(SDL_FPoint& position_p)
{
    SetActive(true);
    position = position_p;

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
    SDL_FRect paddle = scene->GetPaddle().GetRectangle();

    if(Physics::isSphereCollidingWithRect(position, radius, paddle))
    {
        // Find closest point and push ball outside
        float closest_y = SDL_clamp(position.y, paddle.y, paddle.y + paddle.h);
        
        position.y = closest_y - radius;

        triggerEffect();
        Destroy();
    }
    else if(position.y > game_viewport.y + radius)
    {
        Destroy();
    }
}

void Powerup::Destroy()
{
    active = false;
    scene->DestroyObject(this);
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

SDL_Color Powerup::GetColor(Type type_p)
{
    SDL_Color color = { .r = 255, .g = 255, .b = 255, .a = 255 };

    switch (type_p)
    {
    case Type::scoreMultiplier:
        color = {255, 0, 0, 255};
        break; // Red
    case Type::life:
        color = {255, 127, 0, 255};
        break; // Orange
    case Type::spawnBalls:
        color = {255, 255, 0, 255};
        break; // Yellow
    case Type::biggerPaddle:
        color = {0, 255, 0, 255};
        break; // Green
    case Type::fasterPaddle:
        color = {0, 0, 255, 255};
        break; // Blue
    case Type::biggerBall:
        color = {128, 0, 128, 255};
        break; // Purple
    default:
        color = {255, 255, 255, 255};
        break;
    }

    return color;
}

void Powerup::triggerEffect()
{
    switch(type)
    {
        case Type::biggerBall:
            Ball::IncreaseRadius();
            break;
        case Type::fasterPaddle:
            scene->GetPaddle().IncreaseSpeed();
            break;
        case Type::biggerPaddle:
            scene->GetPaddle().IncreaseWidth();
            break;
        case Type::spawnBalls:
            spawnBallsEffect(position);
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

void Powerup::spawnBallsEffect(SDL_FPoint position)
{
    const int ball_count = 3;
    const float angle_step = (2.0f * SDL_PI_F) / ball_count;

    float base_angle = SDL_randf() * 2.0f * SDL_PI_F;

    for (int i = 0; i < ball_count; i++)
    {
        float angle = base_angle + i * angle_step;

        Ball* ball = scene->CreateObject<Ball>(false);

        SDL_FPoint spawn_position = position;
        ball->SetPosition(spawn_position);

        SDL_FPoint direction;
        direction.x = SDL_cosf(angle);
        direction.y = SDL_sinf(angle);
        ball->Launch(direction, 0.5f);
    }
}