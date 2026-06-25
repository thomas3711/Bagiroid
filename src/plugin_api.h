#pragma once
#define PLUGIN_VERSION 1

typedef struct
{ 
    float x, y, w, h;
    uint r, g, b;
    int points;
    int powerup;
}
BrickData;

typedef struct
{
    int count;
    BrickData* bricks;
}
GeneratedBrickskData;