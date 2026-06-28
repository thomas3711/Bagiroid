#include "ball.h"
#include "game.h"
#include "draw.h"
#include "physics.h"

int Ball::radius = Ball::default_radius;

Ball::Ball(bool reduce_lives_after_death_p)
{
    reduce_lives_after_death = reduce_lives_after_death_p;

    game_viewport.x = Game::GetInstance()->game_viewport.w;
    game_viewport.y = Game::GetInstance()->game_viewport.h;
}

Ball::~Ball()
{
    
}

void Ball::Destroy()
{
    scene->DestroyObject(this);
    Game::GetInstance()->NotifyBallDestruction(this);
}

void Ball::IncreaseRadius()
{
    radius = SDL_min(radius + radius_increase, max_radius);
}

void Ball::SetPosition(SDL_FPoint& target_position)
{
    position.x = target_position.x;
    position.y = target_position.y;
}

void Ball::Launch(SDL_FPoint& direction, float speed_factor)
{
    velocity.x = direction.x * speed * speed_factor;
    velocity.y = direction.y * speed * speed_factor;
}

void Ball::Render(SDL_Renderer* renderer)
{
    SDL_Color color = { .r = 255, .g = 255, .b = 255, .a = 255};

    if(!reduce_lives_after_death)
    {
        color = { .r = 255, .g = 255, .b = 0, .a = 255};
    }

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    
    Draw::drawFilledCircle(renderer, position.x, position.y, radius);
}

void Ball::Update(float delta_time)
{
    position.x += velocity.x * delta_time;
    position.y += velocity.y * delta_time;

    // Check collision and bounce against wall
    SDL_FPoint normal = checkScreenEdgeCollision();
    if (normal.x != 0.0f || normal.y != 0.0f) {
        // Clamp ball to stay in bounds
        position.x = SDL_clamp(position.x, radius, Game::GetInstance()->game_viewport.w - radius);
        position.y = SDL_clamp(position.y, radius, Game::GetInstance()->game_viewport.h - radius);
        
        // Calculate and set bounce velocity
        bounce(normal);
    }

    // Check collision and bounce against paddle
    SDL_FRect paddle = scene->GetPaddle().GetRectangle();
    // Squash the paddle rectangle so we don't bounce from sides.
    // As it causes all sorts of weird ball bounce behaviour in
    // extremes that I don't want to deal with.
    paddle.h = 0;

    if(Physics::isSphereCollidingWithRect(position, radius, paddle))
    {
        // Push ball outside the paddle (normal not needed here, paddle has its own bounce)
        resolveRectCollision(paddle);

        // Calculate and set bounce velocity based on where the ball hit the paddle
        bounceOffPaddle(paddle);
    }

    // Check collision with all bricks
    const std::vector<Brick*>& bricks = scene->GetBricks();

    for(int i = 0; i < bricks.size(); i++)
    {
        auto brick = bricks[i];
        auto rectangle = brick->GetRectangle();

        if(brick->IsActive() && Physics::isSphereCollidingWithRect(position, radius, rectangle))
        {
            // Push ball outside the brick and bounce off the resulting normal
            SDL_FPoint normal = resolveRectCollision(rectangle);
            bounce(normal);

            brick->Destroy();
        }
    }

    // Check if ball is below bottom
    if(position.y > game_viewport.y + radius)
    {
        Destroy();
    }
}

SDL_FPoint Ball::checkScreenEdgeCollision()
{
    if (position.x - radius < 0) 
        return {1.0f, 0.0f};      // left wall
    if (position.x + radius > Game::GetInstance()->game_viewport.w) 
        return {-1.0f, 0.0f};     // right wall
    if (position.y - radius < 0) 
        return {0.0f, 1.0f};      // top wall
    
    return {0.0f, 0.0f};          // no collision
}

SDL_FPoint Ball::resolveRectCollision(const SDL_FRect& rect)
{
    // Find closest point on rectangle to circle center
    float closest_x = SDL_clamp(position.x, rect.x, rect.x + rect.w);
    float closest_y = SDL_clamp(position.y, rect.y, rect.y + rect.h);

    // Vector from closest point to ball center (collision normal direction)
    float nx = position.x - closest_x;
    float ny = position.y - closest_y;

    // Normalize the normal vector
    float length = SDL_sqrt(nx*nx + ny*ny);
    if (length > 0.0f) {
        nx /= length;
        ny /= length;
    }

    // Push ball outside the rectangle along the normal
    position.x = closest_x + nx * radius;
    position.y = closest_y + ny * radius;

    return {nx, ny};
}

void Ball::bounce(const SDL_FPoint& normal)
{
    // Simple bounce that follows the rule: The Angle of Incidence equals the Angle of Reflection.

    // Dot product: v · n
    float dot = velocity.x * normal.x + velocity.y * normal.y;

    // Reflection: v - 2(v·n)n
    velocity.x = velocity.x - 2.0f * dot * normal.x;
    velocity.y = velocity.y - 2.0f * dot * normal.y;
}

void Ball::bounceOffPaddle(const SDL_FRect& paddle)
{
    // For game to be fun, player needs to have some sort of control over how the ball bounces off
    // the paddle. Change the angle depending on which side of the paddle the ball hits and how
    // far from the center it is.

    // Where did the ball hit, relative to paddle center? Range [-1, 1].
    float paddle_center = paddle.x + paddle.w * 0.5f;
    float offset = (position.x - paddle_center) / (paddle.w * 0.5f);
    offset = SDL_clamp(offset, -1.0f, 1.0f);

    // Map hit position to an angle measured from straight-up.
    const float max_bounce_angle = 60.0f * (SDL_PI_F / 180.0f); // tune this
    float angle = offset * max_bounce_angle;

    // Always send the ball upward, preserving its speed.
    velocity.x = SDL_sinf(angle) * speed;
    velocity.y = -SDL_cosf(angle) * speed;
}