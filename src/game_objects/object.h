#pragma once
#include <SDL3/SDL.h>

class Scene;

// Parent class for all game objects
class Object
{
protected:
    bool active = true;

    // Doesn't own. Don't touch in constructor, is initialized when added to scene
    Scene* scene = nullptr;

public:
    virtual ~Object() = default;

    virtual void Render(SDL_Renderer* renderer) {};
    virtual void Update(float delta_time) {};

    virtual void SetActive(bool state) { active = state; };
    virtual bool IsActive() const { return active; };

    virtual void Destroy() { };

    void SetScene(Scene* owning_scene) { scene = owning_scene; };
};
