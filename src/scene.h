#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include "object.h"
#include "paddle.h"
#include "brick.h"

class Scene
{
private:
    std::vector<Object*> objects;

    Paddle paddle = Paddle();

    bool debug_print = true;

public:
    Scene();

    void Update(float delta_time);
    void Render(SDL_Renderer* renderer);

    void AddObject(Object* object_to_add);
    void RemoveObject(Object* object_to_remove);
    
    void ResetAllBricks();
    int GetAliveBricksCount();
    Brick* GetBrick(SDL_Point id);
    std::vector<Brick*>& GetBricks();

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