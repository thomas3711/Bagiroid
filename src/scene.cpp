#include "scene.h"
#include <algorithm>

Scene::Scene()
{
    
}

void Scene::Update(float delta_time)
{
    // Paddle
    paddle.Update(delta_time);

    // Balls
    for(int i = 0; i < balls.size(); i++)
    {
        auto ball = balls[i];

        if(ball == nullptr)
        {
            SDL_Log("Error: null pointer ball: (%d)", i);
            break;
        }

        ball->Update(delta_time);
    }
}

void Scene::Render(SDL_Renderer* renderer)
{
    // Bricks
    for(int i = 0; i < bricks.size(); i++)
    {
        auto brick = bricks[i];

        if(brick == nullptr)
        {
            SDL_Log("Error: null pointer brick: (%d)", i);
            break;
        }

        brick->Render(renderer);
    }

    // Balls
    for(int i = 0; i < balls.size(); i++)
    {
        auto ball = balls[i];

        if(ball == nullptr)
        {
            SDL_Log("Error: null pointer ball: (%d)", i);
            break;
        }

        ball->Render(renderer);
    }

    // Paddle
    paddle.Render(renderer);
}

void Scene::AddBall(Ball* ball_to_add)
{
    balls.push_back(ball_to_add);
}

void Scene::AddBrick(Brick* brick_to_add)
{
    bricks.push_back(brick_to_add);
}

void Scene::RemoveBall(Ball* ball_to_remove)
{
    auto iterator = std::find(balls.begin(), balls.end(), ball_to_remove);
    if (iterator != balls.end())
    {
        //delete *iterator;
        balls.erase(iterator);
    }
}

void Scene::RemoveAllBalls()
{
    // Balls
    for(int i = 0; i < balls.size(); i++)
    {
        auto ball = balls[i];

        if(ball == nullptr)
        {
            SDL_Log("Error: null pointer ball: (%d)", i);
            break;
        }

        delete ball;
    }

    balls.clear();
}

void Scene::RespawnAllBricks()
{
    for(int i = 0; i < bricks.size(); i++)
    {
        auto brick = bricks[i];

        if(brick == nullptr)
        {
            SDL_Log("RespawnAllBricks: null brick: %d !", i);
            continue;
        }

        brick->Respawn();
    }
}

int Scene::GetAliveBricksCount()
{
    int total_alive_bricks = 0;

    for(int i = 0; i < bricks.size(); i++)
    {
        auto brick = bricks[i];

        if(brick == nullptr)
        {
            SDL_Log("RespawnAllBricks: null brick: %d !", i);
            continue;
        }

        if(brick->IsAlive())
        {
            total_alive_bricks++;
        }
    }

    return total_alive_bricks;
}