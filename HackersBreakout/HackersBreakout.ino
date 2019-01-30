// Alex Ayala, Pat Falvey, Steven Eisemann
// Hackers in the Bazaar

#include <Arduboy2.h>

Arduboy2 arduboy;
boolean playing = false;
const unsigned int rows = 7;
const unsigned int columns = 13;
boolean bricks[rows][columns];
const unsigned int paddleSize = 10;
const unsigned int paddleX = 10;
boolean ballLaunched = false;
int ballX = 11;
int ballY = 34;
int dX = 0;
int dY = 0;
int paddleY = 30;

void setup() {
  // put your setup code here, to run once:
  arduboy.begin();
  arduboy.setFrameRate(40);
  arduboy.initRandomSeed();
}

void loop() {
  // put your main code here, to run repeatedly:
  while(!playing) {
    if(arduboy.pressed(A_BUTTON)) {
      arduboy.clear();
      playing = true;
    }   
  }
  if (!arduboy.nextFrame()) {
      return;
  }  
  movePaddle();
  moveBall();
  arduboy.display();
}

void movePaddle() {
  arduboy.drawRect(paddleX, paddleY, 1, paddleSize, 0);
  if(arduboy.pressed(UP_BUTTON) && paddleY > 0) {
    paddleY -= 1;
  } else if(arduboy.pressed(DOWN_BUTTON) && paddleY < 54) {
    paddleY += 1;
  }
  arduboy.drawRect(paddleX, paddleY, 1, paddleSize, 1);
}

void moveBall() {
  arduboy.drawRect(ballX, ballY, 2, 2, 0);
  if(arduboy.pressed(B_BUTTON) && !ballLaunched) {
    dX = 1;
    dY = random(0,2)*2-1;
    ballLaunched = true;
  } else if(!ballLaunched) {
    ballX = paddleX + 1;
    ballY = paddleY + 4;
  }
  bounceWall();
  ballX += dX;
  ballY += dY;
  arduboy.drawRect(ballX, ballY, 2, 2, 1);
  bounceWall();
}

void bounceWall() {
  if(ballY < 0) {
    ballY = 0;
    dY = -dY;
  }
  if(ballY > 62) {
    ballY = 62;
    dY = -dY;
  }
  if(ballX > 126) {
    ballX = 126;
    dX = -dX;
  }
  if(ballX < -1) {
    ballX = paddleX+1;
    ballY = paddleY + 4; 
    dX = 0;
    dY = 0;
    ballLaunched = false;
  }
}
