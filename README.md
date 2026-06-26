Game controlls:
A/left arrow - move paddle to the left
D/right arrow - move paddle to the right
Space - launch ball (if there is one on the paddle)
R - restart game after player death

Plugins/Mods:
- Launch the game with parameter -p or --plugins to load mods for level generation
- Game searches for libraries in /plugins subfolder from where the executable is

Architecture:
Game - manages game state, start, end, score, winning conditions, etc
Scene - contains all objects, paddle, bricks and balls
UI - renders ui
Paddle - player controlled paddle
Object - parent class for objects in the game (bricks, balls, powerups) for simpler handling in Scene
Bricks - set of bricks that player shoots at
Ball - ball that hits bricks and bounces of paddle
Powerup - mostly a ball falling from brick to be collected for bonus
LevelGenerator - generates brick field, populates them with powerups

TODO:
Test on windows
Fullscreen - and display resolution detection and error on insufficient resolution.
Documentation - Architecture description in README
cleanup
unify default/start value setting - Macro vs Const vars vs static constexpr
