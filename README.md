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

BUGS:
Issue 3 (the crash) — dangling Paddle::ball pointer
paddle.h:25 holds a raw Ball* to a ball that the scene owns and deletes. When RemoveAllBalls()/RemoveDeadBalls() free the ball the paddle is currently holding (an unlaunched ball 
sitting on the paddle), paddle.ball becomes a dangling pointer. Then:

updateBallPosition() writes through it (paddle.cpp:103) → use-after-free write into freed heap → corrupted double-linked list.
GiveBall() only checks ball == nullptr (paddle.cpp:109), so a dangling (non-null) pointer is never replaced.
Realistic trigger: a ball falls → NotifyBallDestruction → GiveBall() puts a fresh ball on the paddle; in the same frame the flying ball clears the last brick → end-of-frame 
nextLevel() → RemoveAllBalls() frees that held ball → paddle dangles.

Issue 1 — whole cascade resolves in one frame
The ball loop condition is i < balls.size() (scene.cpp:20). SpawnBalls appends during the loop, so freshly spawned balls are updated in the same pass — they hit bricks, spawn more 
balls/powerups, etc. The entire chain reaction collapses into a single frame, so all powerups appear simultaneously. Snapshotting the count spreads it across frames (now safe since 
removals are deferred).

Let me fix all three.

