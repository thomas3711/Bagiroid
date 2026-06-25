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
Fullscreen - and display resolution detection and error on insufficient resolution.
cleanup
unify default/start value setting - Macro vs Const vars

