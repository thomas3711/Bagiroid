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
    // Render all objects
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

    if (dynamic_cast<Brick*>(object_to_add))
    {
        brick_cache_dirty = true;
    }
}

void Scene::RemoveObject(Object* object_to_remove)
{
    auto iterator = std::find(objects.begin(), objects.end(), object_to_remove);
    if (iterator != objects.end())
    {
        objects.erase(iterator);

        if (dynamic_cast<Brick*>(object_to_remove))
        {
            brick_cache_dirty = true;
        }
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
    brick_cache_dirty = true;

    std::queue<Object*> empty;
    std::swap(destroy_queue, empty);
}

void Scene::AddBricks(GeneratedBrickskData* data)
{
    for(int i = 0; i < data->count; i++)
    {
        if(i == MAX_BRICKS)
        {
            return;
        }

        BrickData& brick_data = data->bricks[i];
        
        SDL_FRect rectangle = { .x = brick_data.x, .y = brick_data.y, .w = brick_data.w, .h = brick_data.h };
        SDL_Color color = { .r = (uint8_t)brick_data.r, .g = (uint8_t)brick_data.g, .b = (uint8_t)brick_data.b, .a = 255 };

        Powerup::Type powerup = (Powerup::Type)SDL_clamp(brick_data.powerup, Powerup::Type::none, Powerup::Type::COUNT - 1);
        Brick* brick = new Brick(rectangle, color, brick_data.points, powerup);
        AddObject(brick);
    }
}

const std::vector<Brick*>& Scene::GetBricks()
{
    if (brick_cache_dirty)
    {
        brick_cache.clear();

        for (Object* object : objects)
        {
            if (Brick* brick = dynamic_cast<Brick*>(object))
            {
                brick_cache.push_back(brick);
            }
        }

        brick_cache_dirty = false;
    }

    return brick_cache;
}

int Scene::GetActiveBricksCount()
{
    const std::vector<Brick*>& bricks = GetBricks();

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