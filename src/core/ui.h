#pragma once
#include <SDL3/SDL.h>

struct Player;

// Renders UI
class UI
{
private:
    const SDL_Color info_panel_background_color { .r = 45, .g = 45, .b = 45, .a = 255};

public:
    void RenderRestartGameUI(SDL_Renderer* renderer, const SDL_Rect& viewport, float scale = 2.0f);
    void RenderInfoPanelUI(SDL_Renderer* renderer, const SDL_Rect& viewport, const Player& player, bool game_running, bool plugins_loaded);
};
