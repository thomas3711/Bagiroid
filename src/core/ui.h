#pragma once
#include <SDL3/SDL.h>

struct Player;

// Renders UI
class UI
{
private:
    const SDL_Color info_panel_background_color { .r = 45, .g = 45, .b = 45, .a = 255};
    const SDL_Color game_background_color { .r = 30, .g = 30, .b = 30, .a = 255};

public:
    void RenderRestartGameUI(SDL_Renderer* renderer, const SDL_Rect& viewport, float scale = 2.0f);
    void RenderInfoPanelUI(SDL_Renderer* renderer, const SDL_Rect& viewport, const Player& player, bool game_running, bool plugins_loaded);
    void RenderGameBackground(SDL_Renderer* renderer, const SDL_Rect& viewport);
};
