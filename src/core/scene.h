#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <queue>
#include <memory>
#include <utility>
#include "object.h"
#include "paddle.h"
#include "brick.h"
#include "plugin_api.h"

inline constexpr int MAX_BRICKS = 4096;

// Container and management class for all game objects.
class Scene
{
private:
    std::vector<std::unique_ptr<Object>> objects;

    std::queue<Object*> destroy_queue;

    Paddle paddle = Paddle();

    // Cached list of bricks
    std::vector<Brick*> brick_cache;
    bool brick_cache_dirty = true;

public:
    Scene();

    void Update(float delta_time);
    void Render(SDL_Renderer* renderer);

    Object* AddObject(std::unique_ptr<Object> object_to_add);
    void RemoveObject(Object* object_to_remove);
    void DestroyObject(Object* object_to_destroy);
    void DeleteAll();
    void AddBricks(const GeneratedBricksData* data);

    template <typename T, typename... Args>
    T* CreateObject(Args&&... args)
    {
        auto object = std::make_unique<T>(std::forward<Args>(args)...);
        T* observer = object.get();
        AddObject(std::move(object));
        return observer;
    }

    int GetActiveBricksCount();

    const std::vector<Brick*>& GetBricks();

    Paddle& GetPaddle() { return paddle; };

    template <typename T> void DeleteAllOfType()
    {
        for (auto it = objects.begin(); it != objects.end(); )
        {
            if (dynamic_cast<T*>(it->get()) != nullptr)
            {
                if (!brick_cache_dirty && dynamic_cast<Brick*>(it->get()))
                {
                    brick_cache_dirty = true;
                }

                it = objects.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    template <typename T> std::vector<T*> GetAllOfType()
    {
        std::vector<T*> result;

        for (const std::unique_ptr<Object>& object : objects)
        {
            if (T* casted = dynamic_cast<T*>(object.get()))
            {
                result.push_back(casted);
            }
        }

        return result;
    }
};
