#pragma once
#include <SDL3/SDL.h>

class Object
{
protected:
    bool active;

public:
    virtual ~Object() = default;

    virtual void Render(SDL_Renderer* renderer) {};
    virtual void Update(float delta_time) {};

    virtual void SetActive(bool state) { active = state; };
    virtual bool IsActive() { return active; };

    virtual void Destroy() { };
};