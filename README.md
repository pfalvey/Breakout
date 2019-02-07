# Breakout
For CSE 40842 Project 1

Our project is the game Breakout on the Arduboy, originally designed by Nolan Bushnell, Steve Wosniak, and Steve Bristow in 1976. Unlike the original implementation ours is done in C++, not using TTL chips. Developing for an Arduboy required that we pay more attention to our resource usage, as the device only has 32KB of memory and 2.5KB of RAM. We expanded upon the original game by adding in the chance for a second ball to come into play. For every brick hit there is a 20% chance that a new ball pops out of it, however if there is already a second ball active this effect will not happen. While two balls are in play, losing one will not penalize you a life; you only lose lives if there are no balls in play.

To run the game simply turn on the Arduboy and after the word "Arduboy" appears on the screen press the left red button. This is start the game, showing you the bricks, paddle, and ball on the paddle. Pressing the right red button will launch the ball off of the paddle and truly begin the game. The game will end after you either clear all of the bricks and win, or lose five lives and get a game over.
