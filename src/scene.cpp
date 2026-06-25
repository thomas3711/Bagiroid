#include "scene.h"
#include <algorithm>

Scene::Scene()
{
    
}

void Scene::Update(float delta_time)
{
    // Update all objects
    for(int i = 0; i < objects.size(); i++)
    {
        auto object = objects[i];

        object->Update(delta_time);
    }

    // Paddle
    paddle.Update(delta_time);

    // Process objects that are to be destroyed
    while (!destroy_queue.empty())
    {
        Object* object = destroy_queue.front();
        destroy_queue.pop();
        RemoveObject(object);
        delete object;
    }
}

void Scene::Render(SDL_Renderer* renderer)
{
    for(int i = 0; i < objects.size(); i++)
    {
        auto object = objects[i];

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

void Scene::DestroyObject(Object* object_to_destroy)
{
    destroy_queue.push(object_to_destroy);
}

void Scene::DeleteAll()
{
    for (Object* object : objects)
    {
        delete object;
    }
    objects.clear();

    std::queue<Object*> empty;
    std::swap(destroy_queue, empty);
}

void Scene::AddBricks(GeneratedBrickskData* data)
{
    for(int i = 0; i < data->count; i++)
    {
        BrickData& brick_data = data->bricks[i];
        
        SDL_FRect rectangle = { .x = brick_data.x, .y = brick_data.y, .w = brick_data.w, .h = brick_data.h };
        SDL_Color color = { .r = (uint8_t)brick_data.r, .g = (uint8_t)brick_data.g, .b = (uint8_t)brick_data.b, .a = 255 };

        Brick* brick = new Brick(rectangle, color, brick_data.points, (Powerup::Type)brick_data.powerup);
        AddObject(brick);
    }
}

int Scene::GetActiveBricksCount()
{
    auto bricks = GetAllOfType<Brick>();

    int total_active_bricks = 0;

    for(int i = 0; i < bricks.size(); i++)
    {
        auto brick = bricks[i];

        if(brick->IsActive())
        {
            total_active_bricks++;
        }
    }

    return total_active_bricks;
}