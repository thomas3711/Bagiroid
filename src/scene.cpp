#include "scene.h"
#include <algorithm>

Scene::Scene()
{
    
}

void Scene::Update(float delta_time)
{
    for(int i = 0; i < objects.size(); i++)
    {
        auto object = objects[i];

        if(object == nullptr)
        {
            SDL_Log("Error: null pointer object: (%d)", i);
            continue;
        }

        object->Update(delta_time);
    }

    // Paddle
    paddle.Update(delta_time);
}

void Scene::Render(SDL_Renderer* renderer)
{
    for(int i = 0; i < objects.size(); i++)
    {
        auto object = objects[i];

        if(object == nullptr)
        {
            SDL_Log("Error: null pointer object: (%d)", i);
            continue;
        }

        object->Render(renderer);
    }

    // Paddle
    paddle.Render(renderer);
}

void Scene::AddObject(Object* object_to_add)
{
    objects.push_back(object_to_add);
}

void Scene::RemoveObject(Object* object_to_remove)
{
    auto iterator = std::find(objects.begin(), objects.end(), object_to_remove);
    if (iterator != objects.end())
    {
        objects.erase(iterator);
    }
}

void Scene::ResetAllBricks()
{
    auto bricks = GetAllOfType<Brick>();

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
    auto bricks = GetAllOfType<Brick>();

    int total_alive_bricks = 0;

    for(int i = 0; i < bricks.size(); i++)
    {
        auto brick = bricks[i];

        if(brick == nullptr)
        {
            SDL_Log("RespawnAllBricks: null brick: %d !", i);
            continue;
        }

        if(brick->IsActive())
        {
            total_alive_bricks++;
        }
    }

    return total_alive_bricks;
}

Brick* Scene::GetBrick(SDL_Point id)
{
    auto bricks = GetAllOfType<Brick>();

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