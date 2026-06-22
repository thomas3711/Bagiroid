#include "scene.h"
#include <algorithm>

Scene::Scene()
{
    
}

void Scene::Update(float delta_time)
{
    if(powerups.size() > 0)
    {
        //SDL_Log("Powerups: %d", powerups.size());
    }

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

    // Powerups
    for(int i = 0; i < powerups.size(); i++)
    {
        auto powerup = powerups[i];

        if(powerup == nullptr)
        {
            SDL_Log("Error: null pointer powerup: (%d)", i);
            break;
        }

        powerup->Update(delta_time);
    }
}

void Scene::Render(SDL_Renderer* renderer)
{
    // TODO: group all objects with common parent

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

    // Powerups
    for(int i = 0; i < powerups.size(); i++)
    {
        auto powerup = powerups[i];

        if(powerup == nullptr)
        {
            SDL_Log("Error: null pointer powerup: (%d)", i);
            break;
        }

        powerup->Render(renderer);
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
        balls.erase(iterator);
    }
}

void Scene::RemoveAllBalls()
{
    for(int i = 0; i < balls.size(); i++)
    {
        auto ball = balls[i];

        if(ball == nullptr)
        {
            SDL_Log("Error: null pointer ball: (%d)", i);
            continue;
        }

        delete ball;
    }

    balls.clear();
}

void Scene::AddPowerup(Powerup* powerup_to_add)
{
    powerups.push_back(powerup_to_add);
}

void Scene::RemovePowerup(Powerup* powerup_to_remove)
{
    auto iterator = std::find(powerups.begin(), powerups.end(), powerup_to_remove);
    if (iterator != powerups.end())
    {
        powerups.erase(iterator);
    }
}

void Scene::RemoveAllPowerups()
{
    for(int i = 0; i < powerups.size(); i++)
    {
        auto powerup = powerups[i];

        if(powerup == nullptr)
        {
            SDL_Log("Error: null pointer powerup: (%d)", i);
            continue;
        }

        delete powerup;
    }

    powerups.clear();
}

void Scene::ResetAllBricks()
{
    for(int i = 0; i < bricks.size(); i++)
    {
        auto brick = bricks[i];

        if(brick == nullptr)
        {
            SDL_Log("RespawnAllBricks: null brick: %d !", i);
            continue;
        }

        brick->Reset();
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

Brick* Scene::GetBrick(SDL_Point id)
{
    for(int i = 0; i < bricks.size(); i++)
    {
        Brick* brick = bricks[i];

        if(brick == nullptr)
        {
            SDL_Log("GetBrick: null brick: %d !", i);
            continue;
        }

        if(brick->GetId().x == id.x and brick->GetId().y == id.y)
        {
            return brick;
        }
    }

    return nullptr;
}