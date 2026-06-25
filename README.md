Architecture:
Game - manages game state, start, end, score, winning conditions, etc
Scene - contains all objects, paddle, bricks and balls 
Paddle - player controlled paddle
Object - parent class for objects in the game (bricks, balls, powerups) for simpler handling in Scene
Bricks - set of bricks that player shoots at
Ball - ball that hits bricks and bounces of paddle
Powerup - mostly a ball falling from brick to be collected for bonus

TODO:
remove null pointer checks in Update function in scene - this can't happen anyway
add UI class - encapsulate ui construction
add Help - power up descriptions, movement
cleanup
audio
put all start values/defaults/constants in one config file - unify constant style (macro vs const)

