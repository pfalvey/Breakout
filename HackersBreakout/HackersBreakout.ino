// Alex Ayala, Pat Falvey, Steven Eisemann
// Hackers in the Bazaar

#include <Arduboy2.h>

Arduboy2 arduboy;
BeepPin1 beep;
boolean playing = false;
const unsigned int rows = 7;
const unsigned int columns = 8;
int columnOffset = 128 - columns * 5;
boolean bricks[rows][columns]; // Represents the bricks. False for not hit, true for hit.
const unsigned int paddleSize = 10;
const unsigned int paddleX = 10;
boolean ballLaunched = false; // Checks if ball is moving on its own or needs to be launched
int ballX = 11;
int ballY = 34;
int dX = 0;
int dY = 0;
int paddleY = 30;
int lives = 5;
int hitCount = 0; // How many bricks have been hit
boolean extraBall = false; // Changes to true when an extra ball is found after breaking a block
int extraBallX = 0;
int extraBallY = 0;
int extraDX = 0;
int extraDY = 0;


// Set up
void setup() {
  arduboy.begin();
  beep.begin();
  arduboy.setFrameRate(40);
  arduboy.initRandomSeed();
}

// Main game loop
void loop() {
  // Game over screen
  if (lives == 0) {
    arduboy.clear();
    arduboy.print("Game over");
  }
  // Win screen
  if (hitCount >= columns * rows) {
    arduboy.clear();
    arduboy.print("You win!");
  }
  // Wait until A is pressed to start the game
  while (!playing) {
    if (arduboy.pressed(A_BUTTON)) {
      arduboy.clear();
      initialDrawBlocks();
      playing = true;
    }
  }
  // Ensure that it's time for next frame before doing any movement calculation
  if (!arduboy.nextFrame()) {
    return;
  }

  movePaddle();
  moveBall();
  arduboy.display();
}

// Moves the paddle up and down
void movePaddle() {
  // Initial drawRect clears old position
  arduboy.drawRect(paddleX, paddleY, 1, paddleSize, 0);
  if (arduboy.pressed(UP_BUTTON) && paddleY > 0) {
    paddleY -= 1;
  } else if (arduboy.pressed(DOWN_BUTTON) && paddleY < 54) {
    paddleY += 1;
  }
  arduboy.drawRect(paddleX, paddleY, 1, paddleSize, 1);
}

// Moves the ball and extra ball
void moveBall() {
  // Initial drawRect clears old position
  arduboy.drawRect(ballX, ballY, 2, 2, 0);
  if (extraBall) {
    arduboy.drawRect(extraBallX, extraBallY, 2, 2, 0);
  }
  // If the player presses the launch button give the ball velocity
  if (arduboy.pressed(B_BUTTON) && !ballLaunched) {
    dX = 1;
    dY = random(0, 2) * 2 - 1;
    ballLaunched = true;
  } else if (!ballLaunched) {
    // otherwise have it move with the paddle
    ballX = paddleX + 1;
    ballY = paddleY + 4;
  }

  // Check if there is a wall, paddle, or brick bounce and the effects of it on the ball. Move ball
  // accordingly
  bounceWall();
  ballX += dX;
  ballY += dY;
  if (extraBall) {
    extraBallX += extraDX;
    extraBallY += extraDY;
    arduboy.drawRect(extraBallX, extraBallY, 2, 2, 1);
  }
  arduboy.drawRect(ballX, ballY, 2, 2, 1);
  bounceWall();
}

// Check for bounces
void bounceWall() {
  // Wall bounce
  if (ballY < 0) {
    ballY = 0;
    dY = -dY;
  }
  if (ballY > 62) {
    ballY = 62;
    dY = -dY;
  }
  if (ballX > 126) {
    ballX = 126;
    dX = -dX;
  }
  // Now do same wall checking for extra ball
  if (extraBall) {
    if (extraBallY < 0) {
      extraBallY = 0;
      extraDY = -extraDY;
    }
    if (extraBallY > 62) {
      extraBallY = 62;
      extraDY = -extraDY;
    }
    if (extraBallX > 126) {
      extraBallX = 126;
      extraDX = -extraDX;
    }
  }
  // bounce off paddle
  if (ballX <= 11 && ballX > 9 && dX < 0) {
    if (ballY >= paddleY && ballY <= paddleY + paddleSize) {
      dX = -dX;
      dY = ((ballY - (paddleY + 5)) / 3);
      if (dY == 0) {
        dY = random(0, 2) * 2 - 1;
      }
    }
  }
  // bounce extraBall off paddle
  if (extraBall) {
    if (extraBallX <= 11 && extraBallX > 9 && extraDX < 0) {
      if (extraBallY >= paddleY && extraBallY <= paddleY + paddleSize) {
        extraDX = -extraDX;
        extraDY = ((extraBallY - (paddleY + 5)) / 3);
        if (extraDY == 0) {
          extraDY = random(0, 2) * 2 - 1;
        }
      }
    }
  }
  // lose life when ball is missed
  // if extra ball is in play, no penalty, just lose extra ball
  if (extraBall) {
    if (ballX < -1) {
      //Make the extraBall the real ball
      ballX = extraBallX;
      ballY = extraBallY;
      dX = extraDX;
      dY = extraDY;
      extraBallX = 0;
      extraBallY = 0;
      extraDX = 0;
      extraDY = 0;
      extraBall = false;
    }
    if (extraBallX < -1) {
      // if the extraBall dies, just make it disappear
      extraBallX = 0;
      extraBallY = 0;
      extraDX = 0;
      extraDY = 0;
      extraBall = false;
    }
  }
  // if no extra ball lose life, reset ball
  else {
    if (ballX < -1) {
      ballX = paddleX + 1;
      ballY = paddleY + 4;
      dX = 0;
      dY = 0;
      ballLaunched = false;
      lives -= 1;
    }
  }
  // Check for hitting a brick
  // Parts of this logic was borrowed from the Arduboy2 example program, ArduBreakout
  // Check every brick
  for (int row = 0; row < rows; row ++) {
    for (int column = 0; column < columns; column++) {
      // if brick hasn't been hit
      if (!bricks[row][column]) {
        // brick bounds
        int left = columnOffset + 5 * column;
        int right = columnOffset + 5 * column + 4;
        int top = 9 * row;
        int bottom = 9 * row + 8;

        // collision
        if (ballY <= bottom && ballY + 2 >= top && ballX <= right && ballX + 2 >= left) {
          bricks[row][column] = true;
          hitCount += 1;
          arduboy.drawRect(columnOffset + 5 * column, 9 * row, 4, 8, 0);
          // roll to see if an extra ball can be added
          if (random(0, 6) < 2 && extraBall == false) {
            extraBall = true;
            extraBallX = left;
            extraBallY = top;
            extraDX = 1;
            extraDY = random(0, 2) * 2 - 1;
          }

          // horizontal bounce
          if (ballX < left || ballX + 2 > right) {
            dX = - dX;
          }
          // vertical bounce
          if (ballY + 2 > bottom || ballY < top) {
            dY = - dY;
          }
        }
      }
      // Same as above but for extra ball
      if (!bricks[row][column]) {
        // brick bounds
        int left = columnOffset + 5 * column;
        int right = columnOffset + 5 * column + 4;
        int top = 9 * row;
        int bottom = 9 * row + 8;

        // now check for extra ball block collisions
        if (extraBallY <= bottom && extraBallY + 2 >= top && extraBallX <= right && extraBallX + 2 >= left) {
          bricks[row][column] = true;
          hitCount += 1;
          arduboy.drawRect(columnOffset + 5 * column, 9 * row, 4, 8, 0);
          // horizontal bounce
          if (extraBallX < left || extraBallX + 2 > right) {
            extraDX = - extraDX;
          }

          // vertical bounce
          if (extraBallY + 2 > bottom || extraBallY < top) {
            extraDY = - extraDY;
          }
        }
      }
    }
  }
}

// Set up the blocks
void initialDrawBlocks() {
  for (int row = 0; row < rows; row ++) {
    for (int column = 0; column < columns; column++) {
      bricks[row][column] = false;
      arduboy.drawRect(columnOffset + 5 * column, 9 * row, 4, 8, 1);
    }
  }
}
