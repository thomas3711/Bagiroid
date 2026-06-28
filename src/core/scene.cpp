#include "scene.h"
#include <algorithm>

Scene::Scene()
{
    paddle.SetScene(this);
}

void Scene::Update(float delta_time)
{
    // Update all objects
    for (size_t i = 0; i < objects.size(); i++)
    {
        objects[i]->Update(delta_time);
    }

    // Paddle
    paddle.Update(delta_time);

    // Process objects that are to be destroyed
    while (!destroy_queue.empty())
    {
        Object* object = destroy_queue.front();
        destroy_queue.pop();
        RemoveObject(object);
    }
}

void Scene::Render(SDL_Renderer* renderer)
{
    // Render all objects
    for (size_t i = 0; i < objects.size(); i++)
    {
        objects[i]->Render(renderer);
    }

    // Paddle
    paddle.Render(renderer);
}

Object* Scene::AddObject(std::unique_ptr<Object> object_to_add)
{
    Object* observer = object_to_add.get();
    observer->SetScene(this);

    if (dynamic_cast<Brick*>(observer))
    {
        brick_cache_dirty = true;
    }

    objects.push_back(std::move(object_to_add));
    return observer;
}

void Scene::RemoveObject(Object* object_to_remove)
{
    auto iterator = std::find_if(objects.begin(), objects.end(),
        [&](const std::unique_ptr<Object>& object) { return object.get() == object_to_remove; });

    if (iterator != objects.end())
    {
        if (dynamic_cast<Brick*>(object_to_remove))
        {
            brick_cache_dirty = true;
        }

        objects.erase(iterator);
    }
}

void Scene::DestroyObject(Object* object_to_destroy)
{
    destroy_queue.push(object_to_destroy);
}

void Scene::DeleteAll()
{
    objects.clear();
    brick_cache_dirty = true;

    std::queue<Object*> empty;
    std::swap(destroy_queue, empty);
}

void Scene::AddBricks(const GeneratedBricksData* data)
{
    for (int i = 0; i < data->count; i++)
    {
        if (i == MAX_BRICKS)
        {
            SDL_Log("Brick count (%d) exceeds MAX_BRICKS (%d), dropping the rest", data->count, MAX_BRICKS);
            return;
        }

        BrickData& brick_data = data->bricks[i];

        SDL_FRect rectangle = { .x = brick_data.x, .y = brick_data.y, .w = brick_data.w, .h = brick_data.h };
        SDL_Color color = { .r = (uint8_t)brick_data.r, .g = (uint8_t)brick_data.g, .b = (uint8_t)brick_data.b, .a = 255 };

        Powerup::Type powerup = (Powerup::Type)SDL_clamp(brick_data.powerup, Powerup::Type::none, Powerup::Type::COUNT - 1);
        CreateObject<Brick>(rectangle, color, brick_data.points, powerup);
    }
}

const std::vector<Brick*>& Scene::GetBricks()
{
    if (brick_cache_dirty)
    {
        brick_cache.clear();

        for (const std::unique_ptr<Object>& object : objects)
        {
            if (Brick* brick = dynamic_cast<Brick*>(object.get()))
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

    for (size_t i = 0; i < bricks.size(); i++)
    {
        if (bricks[i]->IsActive())
        {
            total_active_bricks++;
        }
    }

    return total_active_bricks;
}
