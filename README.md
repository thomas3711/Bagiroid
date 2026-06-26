# Bagiroid

Arkanoid clone project for hiring homework

## Requirements
- A C++20 compiler
- [SDL3](https://github.com/libsdl-org/SDL) (development libraries)
- CMake 3.16+

## Building
Bagiroid uses CMake. SDL3 must be installed and discoverable by CMake (e.g. via your distro package manager or built from source).

```sh
cmake -B build
cmake --build build
```

This produces the `bagiroid` executable in `build/`, and builds the example plugin into `build/plugins/`.

## Running
```sh
./build/bagiroid            # run the game
./build/bagiroid --plugins  # run with level-generation plugins loaded (see Plugins/Mods)
```

## Game controls
- A/left arrow - move paddle to the left
- D/right arrow - move paddle to the right
- Space - launch ball (if there is one on the paddle)
- R - restart game after player death
- Esc - quit game

## Plugins/Mods
- Launch the game with parameter -p or --plugins to load mods for level generation
- Game searches for libraries in /plugins subfolder from where the executable is
- There is an example plugin with different brick layout in plugins/example/example_plugin.cpp
- Plugin level generation is not very safe, game can definitely be broken by them

## Potential Improvements
- Smooth/Antialiased ball drawing
- Faster collision checks (every ball checks all the bricks) - quadtree
- Expose other settings (player, paddle, ball) to plugins

## Architecture

```
src/
  main.cpp                        - entry point: initializes game and runs main loop

  core/
    game.cpp / game.h             - Game: manages game state, start, end, score, winning conditions, etc
    player.h                      - Player: player state (lives, score, level, score multiplier)
    scene.cpp / scene.h           - Scene: contains all objects, paddle, bricks and balls
    ui.cpp / ui.h                 - UI: renders ui
    level_generator.cpp / .h      - LevelGenerator: generates brick field, populates them with powerups (built-in or via plugin)

  game_objects/
    object.h                      - Object: parent class for objects in the game (bricks, balls, powerups) for simpler handling in Scene
    paddle.cpp / paddle.h         - Paddle: player controlled paddle
    brick.cpp / brick.h           - Brick: a brick that player shoots at
    ball.cpp / ball.h             - Ball: ball that hits bricks and bounces of paddle
    powerup.cpp / powerup.h       - Powerup: mostly a ball falling from brick to be collected for bonus

  plugin_api/
    plugin_api.h                  - API between the game and plugins (brick data structs, exported symbols)
    plugin_loader.cpp / .h        - PluginLoader: loads a plugin shared library and resolves its generate/free functions

  utility/
    draw.cpp / draw.h             - drawing helpers
    physics.cpp / physics.h       - physics/collision helpers

plugins/
  example/example_plugin.cpp      - example level-generation plugin built as a standalone shared library
```
