#include "ball.h"
#include "game.h"
#include "draw.h"
#include "physics.h"

int Ball::radius = Ball::default_radius;

Ball::Ball()
{
    Game::GetInstance()->GetScene()->AddBall(this);
    reduce_lives_after_death = false;
}

Ball::Ball(bool reduce_lives_after_death_p)
{
    Game::GetInstance()->GetScene()->AddBall(this);
    reduce_lives_after_death = reduce_lives_after_death_p;
}

Ball::~Ball()
{
    
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

void Ball::Launch(SDL_FPoint& direction)
{
    velocity.x = direction.x * speed;
    velocity.y = direction.y * speed;
}

void Ball::Render(SDL_Renderer* renderer)
{
    int gray = 255;

    if(!reduce_lives_after_death)
    {
        gray = 192;
    }

    SDL_SetRenderDrawColor(renderer, gray, gray, gray, gray);
    
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
        position.x = SDL_clamp(position.x, radius, Game::GetInstance()->gameViewport.w - radius);
        position.y = SDL_clamp(position.y, radius, Game::GetInstance()->gameViewport.h - radius);
        
        // Calculate and set bounce velocity
        bounce(normal);
    }

    // Check collision and bounce against paddle
    SDL_FRect paddle = Game::GetInstance()->GetScene()->GetPaddle().GetRectangle();
    // Squash the paddle rectangle so we don't bounce from sides.
    // As it caues all sorts of weird ball bounce behaviour in
    // extremes that I don't want to deal with.
    paddle.h = 0;

    if(Physics::isSphereCollidingWithRect(position, radius, paddle))
    {
        // TODO: encapsulate to collider with rectangle (or something so it is not copied in brick check)
        SDL_FPoint normal = getRectCollisionNormal(paddle);
        
        // Find closest point and push ball outside
        float closest_x = SDL_clamp(position.x, paddle.x, paddle.x + paddle.w);
        float closest_y = SDL_clamp(position.y, paddle.y, paddle.y + paddle.h);

        position.x = closest_x + normal.x * radius;
        position.y = closest_y + normal.y * radius;

        // Calculate and set bounce velocity based on where the ball hit the paddle
        bounceOffPaddle(paddle);
    }

    // Check collision with all bricks
    auto bricks = Game::GetInstance()->GetScene()->GetBricks();

    for(int i = 0; i < bricks.size(); i++)
    {
        auto brick = bricks[i];
        auto rectangle = brick->GetRectangle();

        if(brick->IsAlive() and Physics::isSphereCollidingWithRect(position, radius, rectangle))
        {
            // TODO: encapsulate to collider with rectangle (or something so it is not copied in paddle check)
            SDL_FPoint normal = getRectCollisionNormal(rectangle);
            
            // Find closest point and push ball outside
            float closest_x = SDL_clamp(position.x, rectangle.x, rectangle.x + rectangle.w);
            float closest_y = SDL_clamp(position.y, rectangle.y, rectangle.y + rectangle.h);

            position.x = closest_x + normal.x * radius;
            position.y = closest_y + normal.y * radius;
            
            // Calculate and set bounce velocity
            bounce(normal);

            brick->Kill();
        }
    }

    // Check if ball is bellow bottom 
    if(position.y > Game::GetInstance()->window_height + radius)
    {
        Game::GetInstance()->NotifyBallDestruction(this);
    }
}

SDL_FPoint Ball::checkScreenEdgeCollision()
{
    if (position.x - radius < 0) 
        return {1.0f, 0.0f};      // left wall
    if (position.x + radius > Game::GetInstance()->gameViewport.w) 
        return {-1.0f, 0.0f};     // right wall
    if (position.y - radius < 0) 
        return {0.0f, 1.0f};      // top wall
    /*if (position.y + ball_radius > Game::GetInstance()->window_height)
        return {0.0f, -1.0f};     // bottom wall*/
    
    return {0.0f, 0.0f};          // no collision
}

// Calculate rectangle collision normal
SDL_FPoint Ball::getRectCollisionNormal(const SDL_FRect& rect)
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
    
    return {nx, ny};
}

void Ball::bounce(SDL_FPoint& normal)
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