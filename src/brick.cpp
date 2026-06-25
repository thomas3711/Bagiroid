#include "brick.h"
#include "game.h"

Brick::Brick()
{

}

Brick::Brick(SDL_FRect rectangle_p, SDL_Color color_p, uint32_t points_p, Powerup::Type powerup_type_p)
{
    rectangle = rectangle_p;
    color = color_p;
    points = points_p;
    powerup_type = powerup_type_p;

    active = true;
}

void Brick::Render(SDL_Renderer* renderer)
{
    if(active)
    {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(renderer, &rectangle);
    }
}

void Brick::Update(float delta_time)
{

}

void Brick::Destroy()
{
    SetActive(false);
    Game::GetInstance()->NotifyBrickDestruction(this);
    Game::GetInstance()->GetScene()->DestroyObject(this);

    if(powerup_type > Powerup::Type::none && powerup_type < Powerup::Type::COUNT)
    {
        Powerup* powerup = new Powerup(powerup_type);

        if(powerup != nullptr)
        {
            SDL_FPoint position { .x = rectangle.x + rectangle.w / 2.0f, .y = rectangle.y + rectangle.h / 2.0f };
            powerup->Spawn(position);
            powerup_type = Powerup::Type::none;
        }
    }
}