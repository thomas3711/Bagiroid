#pragma once
#define PLUGIN_VERSION 1

#ifdef __cplusplus
extern "C"
{
#endif

// Data
typedef struct
{
    float x, y, w, h;
    unsigned int r, g, b;
    int points;
    int powerup;
}
BrickData;

typedef struct
{
    int count;
    BrickData* bricks;
}
GeneratedBricksData;

typedef struct
{
    int game_width;
    int game_height;
}
BrickGenContext;

// Function names
#define PLUGIN_API_VERSION_SYMBOL "plugin_api_version"
#define PLUGIN_GENERATE_SYMBOL    "plugin_generate_bricks"
#define PLUGIN_FREE_SYMBOL        "plugin_free_bricks"

// Define types for pointers to functions
typedef int (*PluginApiVersionFn)(void);
typedef GeneratedBricksData* (*PluginGenerateFn)(const BrickGenContext* ctx);
typedef void (*PluginFreeFn)(GeneratedBricksData* data);

// Convenience export macros
#if defined(_WIN32)
    #define PLUGIN_EXPORT extern "C" __declspec(dllexport)
#else
    #define PLUGIN_EXPORT extern "C" __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
}
#endif
