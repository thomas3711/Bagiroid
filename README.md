Architecture:
Paddle - player controlled paddle
Bricks - set of bricks that player shoots at
Ball - ball that hits bricks and bounces of paddle
Scene - contains all objects, paddle, bricks and balls
Game - manages game state, start, end, score, winning conditions, etc

TODO:
BUG: - when I restarted game after loosing all lives - ball was blinking (can't reproduce :( maybe reset balls in scene after death ?))
end game - finish level when all bricks are gone 
study the vectors defined by angle and velocity - how does it use sin and cos, understand it better 
cleanup
audio
physics reimplement - research
put all constants in one config file

Power ups:
doubling of point reward
bigger paddle
bigger ball
multiple balls
life
paddle speed
faster ball
faster paddle
balls bounce from bottom

