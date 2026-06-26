#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <queue>
#include "object.h"
#include "paddle.h"
#include "brick.h"
#include "plugin_api.h"

#define MAX_BRICKS 512

// Container and management class for all game objects.
class Scene
{
private:
    std::vector<Object*> objects;

    std::queue<Object*> destroy_queue;

    Paddle paddle = Paddle();

    bool debug_print = true;

public:
    Scene();

    void Update(float delta_time);
    void Render(SDL_Renderer* renderer);

    void AddObject(Object* object_to_add);
    void RemoveObject(Object* object_to_remove);
    void DestroyObject(Object* object_to_destroy);
    void DeleteAll();
    void AddBricks(GeneratedBrickskData* data);
    
    int GetActiveBricksCount();

    Paddle& GetPaddle() { return paddle; };

    template <typename T> void DeleteAllOfType()
    {
        for (auto it = objects.begin(); it != objects.end(); )
        {
            if (dynamic_cast<T*>(*it) != nullptr)
            {
                delete *it;
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

        for (Object* object : objects)
        {
            if (T* casted = dynamic_cast<T*>(object))
            {
                result.push_back(casted);
            }
        }

        return result;
    }
};