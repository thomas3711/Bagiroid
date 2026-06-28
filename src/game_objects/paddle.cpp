#include "paddle.h"
#include "game.h"

Paddle::Paddle()
{
    rectangle.w = default_width;
    rectangle.h = height;

    game_viewport.x = Game::GetInstance()->game_viewport.w;
    game_viewport.y = Game::GetInstance()->game_viewport.h;

    x_min_pos = 0;
    x_max_pos = game_viewport.x - rectangle.w;

    setStartPosition();
}

void Paddle::Render(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, color, color, color, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &rectangle);
}

void Paddle::Update(float delta_time)
{
    if(!control_enabled)
    {
        return;
    }

    const bool *keys = SDL_GetKeyboardState(nullptr);
    int paddle_direction = 0;

    // Ball
    updateBallPosition();
    
    // Left - Right movement
    if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT])
    {
         rectangle.x -= speed * delta_time;
         paddle_direction = -1;
    }
    else if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT]) 
    {
        rectangle.x += speed * delta_time;
        paddle_direction = 1;
    }

    // TODO: encapsulate this
    // Test space key press / release    
    static bool space_was_down = false;
    bool space_down = keys[SDL_SCANCODE_SPACE];

    bool space_key_just_pressed = false;
    bool space_key_released = false;

    if (space_down && !space_was_down)
    {
        space_key_just_pressed = true;
    }
    else if (!space_down && space_was_down)
    {
        space_key_released = true;
    }

    space_was_down = space_down;

    // Shooting ball
    if (ball != nullptr && space_key_just_pressed)
    {
        SDL_FPoint ball_direction;
        ball_direction.x = paddle_direction;
        ball_direction.y = -1;

        ball->Launch(ball_direction, 1.0f);
        ball = nullptr;
    }
    
    // Limit X position
    if(rectangle.x > x_max_pos)
    {
        rectangle.x = x_max_pos;
    }
    else if(rectangle.x < x_min_pos)
    {
        rectangle.x = x_min_pos;
    }
}

void Paddle::setStartPosition()
{
    rectangle.x = game_viewport.x / 2 - rectangle.w / 2.0f;
    rectangle.y = game_viewport.y - height * 2;
}

void Paddle::updateBallPosition()
{
    if(ball != nullptr)
    {
        ball_position.x = GetPositionX() + rectangle.w / 2;
        ball_position.y = GetPositionY() - ball->GetRadius();

        ball->SetPosition(ball_position);
    }
}

void Paddle::GiveBall()
{
    if(ball == nullptr)
    {
        ball = scene->CreateObject<Ball>();

        updateBallPosition();
    }
}

void Paddle::ResetToDefault()
{
    rectangle.w = default_width;
    rectangle.h = height;

    speed = default_speed;
}

void Paddle::IncreaseWidth()
{
    rectangle.w += width_increase;

    // Limit to size of the viewport
    auto limit = game_viewport.x / 2;
    if(rectangle.w > limit)
    {
        rectangle.w = limit;
    }
    else
    {
        rectangle.x -= width_increase / 2.0f;
    }

    x_max_pos = game_viewport.x - rectangle.w;
}

void Paddle::IncreaseSpeed()
{
    speed += speed_increase;

    speed = SDL_min(speed, max_speed);
}