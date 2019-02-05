// Alex Ayala, Pat Falvey, Steven Eisemann
// Hackers in the Bazaar

#include <Arduboy2.h>

Arduboy2 arduboy;
BeepPin1 beep;
boolean playing = false;
const unsigned int rows = 7;
const unsigned int columns = 8;
int columnOffset = 128 - columns*5;
boolean bricks[rows][columns]; //false for not hit
const unsigned int paddleSize = 10;
const unsigned int paddleX = 10;
boolean ballLaunched = false;
int ballX = 11;
int ballY = 34;
int dX = 0;
int dY = 0;
int paddleY = 30;
int lives = 5;
int hitCount = 0;


void setup() {
  // put your setup code here, to run once:
  arduboy.begin();
  beep.begin();
  arduboy.setFrameRate(40);
  arduboy.initRandomSeed();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (lives == 0){
    arduboy.clear();
    arduboy.print("Game over");
  }
  if (hitCount == columns * rows){
    arduboy.clear();
    arduboy.print("You win!");
  }
  while(!playing) {
    if(arduboy.pressed(A_BUTTON)) {
      arduboy.clear();
      initialDrawBlocks();
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
  //bounce off paddle
  if (ballX <= 11 && ballX > 9 && dX < 0){
    if (ballY >= paddleY && ballY <= paddleY + paddleSize){
      dX = -dX;
      dY = ((ballY-(paddleY+5))/3);
      if (dY == 0){
        dY = random(0,2)*2-1;
      }
    }
  }
  //lose life when bottom hit
  if(ballX < -1) {
    ballX = paddleX+1;
    ballY = paddleY + 4; 
    dX = 0;
    dY = 0;
    ballLaunched = false;
    lives -= 1;
  }
  //hit a brick
  //parts of this logic taken from ArduBreakout
  for (int row = 0; row < rows; row ++){
    for (int column = 0; column < columns; column++){
      if (!bricks[row][column]){
        //brick bounds
        int left = columnOffset+5*column;
        int right = columnOffset+5*column + 4;
        int top = 9 * row;
        int bottom = 9 * row + 8;

        //collision
        if (ballY <= bottom && ballY + 2 >= top && ballX <= right && ballX + 2 >= left){
          bricks[row][column] = true;
          hitCount += 1;
          arduboy.drawRect(columnOffset+5*column, 9*row, 4, 8, 0);

          //horizontal bounce
          if (ballX < left || ballX + 2 > right){
            dX =- dX;            
          }

          //vertical bounce
          if (ballY + 2 > bottom || ballY < top){
            dY =- dY;
          }
        }
      }
    }
  }
}

void initialDrawBlocks(){
  for (int row = 0; row < rows; row ++){
    for (int column = 0; column < columns; column++){
      bricks[row][column] = false;
      arduboy.drawRect(columnOffset+5*column, 9*row, 4, 8, 1);
    }
  }
}
