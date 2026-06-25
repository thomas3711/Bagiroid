#include "ui.h"
#include "game.h"
#include "powerup.h"
#include "brick.h"
#include "draw.h"

void UI::RenderRestartGameUI(SDL_Renderer* renderer, const SDL_Rect& viewport, float scale)
{
    const float game_render_scale = 2.0f;
    SDL_SetRenderScale(renderer, game_render_scale, game_render_scale);

    SDL_Rect scaledViewport = {
        (int)(viewport.x / game_render_scale),
        (int)(viewport.y / game_render_scale),
        (int)(viewport.w / game_render_scale),
        (int)(viewport.h / game_render_scale)
    };

    SDL_SetRenderViewport(renderer, &scaledViewport);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    const char *msg = "Press [R] to restart the game.";
    float msg_width = SDL_strlen(msg) * SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE;

    float center_x = viewport.w / game_render_scale / 2.0f;
    float center_y = viewport.h / game_render_scale / 2.0f;

    SDL_RenderDebugText(renderer,
        center_x - msg_width / 2.0f,
        center_y - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE / 2.0f,
        msg
    );

    // Reset back the scale
    SDL_SetRenderViewport(renderer, &viewport);
    SDL_SetRenderScale(renderer, 1.0f, 1.0f);
}

void UI::RenderInfoPanelUI(SDL_Renderer* renderer, const SDL_Rect& viewport, const Player& player, bool game_running)
{
    SDL_SetRenderScale(renderer, 1.0f, 1.0f);
    SDL_SetRenderViewport(renderer, &viewport);

    SDL_SetRenderDrawColor(renderer, info_panel_background_color.r, info_panel_background_color.g, info_panel_background_color.b, info_panel_background_color.a);
    SDL_FRect hud_background = {0.0f, 0.0f, (float)viewport.w, (float)viewport.h};
    SDL_RenderFillRect(renderer, &hud_background);

    // Scale the veiwport because debug text is too small
    const float scale = 2.0f;
    SDL_SetRenderScale(renderer, scale, scale);

    SDL_Rect scaledViewport = {
        (int)(viewport.x / scale),
        (int)(viewport.y / scale),
        (int)(viewport.w / scale),
        (int)(viewport.h / scale)
    };

    SDL_SetRenderViewport(renderer, &scaledViewport);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    const float row_spacing = 16.0f;
    SDL_FPoint ui_position = {10.0f, 10.0f};

    if(game_running)
    {
        // Stats
        SDL_RenderDebugTextFormat(renderer, ui_position.x, ui_position.y, "Level: %d", player.level);
        ui_position.y += row_spacing;
        SDL_RenderDebugTextFormat(renderer, ui_position.x, ui_position.y, "Score: %d", player.score);
        ui_position.y += row_spacing;
        SDL_RenderDebugTextFormat(renderer, ui_position.x, ui_position.y, "Lives: %d", player.lives);
        ui_position.y += row_spacing;
        SDL_RenderDebugTextFormat(renderer, ui_position.x, ui_position.y, "Score Multiplier: %dx", player.score_multiplier);

        // Help
        float hint_row_spacing = 24.0f;
        int hint_circle_radius = 8;
        ui_position.x += hint_circle_radius;
        ui_position.y = scaledViewport.h - 26.0f - hint_row_spacing * Powerup::Type::COUNT;

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDebugTextFormat(renderer, ui_position.x + hint_circle_radius * 2, ui_position.y, "Powerups: ");
        ui_position.y += hint_row_spacing;

        for (int i = 0; i < static_cast<int>(Powerup::Type::COUNT); i++)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            Powerup::Type powerup_type = (Powerup::Type)i;
            std::string description = "- " + Powerup::GetDescription(powerup_type);
            SDL_RenderDebugTextFormat(renderer, ui_position.x + hint_circle_radius * 2, ui_position.y, description.c_str());
            SDL_Color color = Powerup::GetColor(powerup_type);
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            Draw::drawFilledCircle(renderer, ui_position.x, ui_position.y + hint_circle_radius / 2, hint_circle_radius);
            ui_position.y += hint_row_spacing;
        }
    }
    else
    {
        SDL_RenderDebugTextFormat(renderer, ui_position.x, ui_position.y, "You are dead !");
    }

    SDL_SetRenderScale(renderer, 1.0f, 1.0f);
    SDL_SetRenderViewport(renderer, &viewport);
}