#pragma once
#include <SDL3/SDL.h>

#define PLAYER_START_LIVES 3;

struct Player
{
    int lives = PLAYER_START_LIVES;
    uint64_t score = 0;
    uint16_t level = 1;
    uint64_t score_multiplier = 1;
};