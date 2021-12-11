#include <TVout.h>
#include <fontALL.h>

#include "logo.h"

// --------------------------------------------------------------- //
// ------------------------- user config ------------------------- //
// --------------------------------------------------------------- //

// there are defined all the pins
struct Pin {
  static
  const short joystickX = A2; // joystick X axis pin
  static
  const short joystickY = A3; // joystick Y axis pin
  static
  const short potentiometer = A4; // potentiometer for snake speed control
};

// --------------------------------------------------------------- //
// ------------------------------- variables --------------------- //
// --------------------------------------------------------------- //

TVout TV;

struct Point {
  int row = 0, col = 0;
  Point(int row = 0, int col = 0): row(row), col(col) {}
};

struct Coordinate {
  int x = 0, y = 0;
  Coordinate(int x = 0, int y = 0): x(x), y(y) {}
};

bool win = false;
bool gameOver;

// primary snake head coordinates (snake head), it will be randomly generated
Point snake;

// food is not anywhere yet
Point food(-1, -1);

// construct with default values in case the user turns off the calibration
Coordinate joystickHome(500, 500);

// snake parameters
int snakeSpeed = 1; // will be set according to potentiometer value, cannot be 0
int snakeDirection = 0; // if it is 0, the snake does not move
int previousDirection = snakeDirection; // save the last direction
long timestamp;

// direction constants
const short up = 1;
const short right = 2;
const short down = 3; // 'down - 2' must be 'up'
const short left = 4; // 'left - 2' must be 'right'

// threshold where movement of the joystick will be accepted
const int joystickThreshold = 160;

//height and width of board
const int height = 18;
const int width = 22;

// snake segment coordinates
const int maxSnakeLength = 50;
int snakeSegmentsX[maxSnakeLength] = {};
int snakeSegmentsY[maxSnakeLength] = {};
int snakeLength = 1;

void setup() {
  Serial.begin(115200); // set the same baud rate on your Serial Monitor
  randomSeed(analogRead(0));

  TV.begin(NTSC, 120, 96);
  //TV.begin(PAL, 120, 96);

  calibrateJoystick(); // calibrate the joystick home (do not touch it)
  
  intro();
}

void loop() {

  initGame();
  
  while (snakeDirection == 0){
    handleInput();
  }
  
  TV.clear_screen();
  TV.draw_rect(snake.col*5, snake.row*5, 5, 5, WHITE, INVERT);
  TV.draw_rect(food.col*5+1, food.row*5+1, 3, 3, WHITE, INVERT);

  gameLoop();

  TV.clear_screen();

  outtro();
 
}

// --------------------------------------------------------------- //
// -------------------------- functions -------------------------- //
// --------------------------------------------------------------- //

//snake animation
void intro() {
  unsigned char w, l, wb;
  int index;
  w = pgm_read_byte(SnakeLogo);
  l = pgm_read_byte(SnakeLogo + 1);
  if (w & 7)
    wb = w / 8 + 1;
  else
    wb = w / 8;
  index = wb * (l - 1) + 2;
  for (unsigned char i = 1; i < l; i++) {
    TV.bitmap((TV.hres() - w) / 2, 0, SnakeLogo, index, w, i);
    index -= wb;
    TV.delay(50);
  }
  for (unsigned char i = 0; i < (TV.vres() - l) / 2; i++) {
    TV.bitmap((TV.hres() - w) / 2, i, SnakeLogo);
    TV.delay(50);
  }
  TV.delay(3000);
  TV.clear_screen();
}

//game over animation
void outtro() {
  unsigned char w, l, wb;
  int index;
  w = pgm_read_byte(GameOverLogo);
  l = pgm_read_byte(GameOverLogo+ 1);
  if (w & 7)
    wb = w / 8 + 1;
  else
    wb = w / 8;
  index = wb * (l - 1) + 2;
  for (unsigned char i = 1; i < l; i++) {
    TV.bitmap((TV.hres() - w) / 2, 0, GameOverLogo, index, w, i);
    index -= wb;
    TV.delay(50);
  }
  for (unsigned char i = 0; i < (TV.vres() - l) / 2; i++) {
    TV.bitmap((TV.hres() - w) / 2, i, GameOverLogo);
    TV.delay(50);
  }
  TV.delay(3000);
  TV.clear_screen();
}

// standard map function, but with floats
float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// calibrate the joystick home for 10 times
void calibrateJoystick() {
  int x = 0;
  int y = 0;

  for (int i = 0; i < 10; i++) {
    x += analogRead(Pin::joystickX);
    y += analogRead(Pin::joystickY);
  }

  joystickHome.x = x / 10;
  joystickHome.y = y / 10;
}

// causes the snake to appear on the other side of the screen if it gets out of the edge
void fixEdge() {
  snake.col < 0 ? snake.col += width : 0;
  snake.col > width-1 ? snake.col -= width : 0;
  snake.row < 0 ? snake.row += height : 0;
  snake.row > height-1 ? snake.row -= height : 0;
}

//check if point is within snake
bool checkCollision(Point pos) {
  for (int i=0; i < snakeLength - 1; i++) {
    if (snakeSegmentsX[i] == pos.col && snakeSegmentsY[i] == pos.row)
      return true;
  }
  return false;
}

//handle input from potentiometer and joystick
void handleInput() {
  previousDirection = snakeDirection; // save the last direction
  timestamp = millis();

  while (millis() < timestamp + snakeSpeed) {
    // calculate snake speed exponentially (10...1000ms)
    float raw = mapf(analogRead(Pin::potentiometer), 0, 1023, 0, 1);
    snakeSpeed = mapf(pow(raw, 3.5), 0, 1, 10, 1000); // change the speed exponentially
    if (snakeSpeed == 0) snakeSpeed = 1; // safety: speed can not be 0

    // determine the direction of the snake
    analogRead(Pin::joystickY) < joystickHome.y - joystickThreshold ? snakeDirection = up : 0;
    analogRead(Pin::joystickY) > joystickHome.y + joystickThreshold ? snakeDirection = down : 0;
    analogRead(Pin::joystickX) < joystickHome.x - joystickThreshold ? snakeDirection = left : 0;
    analogRead(Pin::joystickX) > joystickHome.x + joystickThreshold ? snakeDirection = right : 0;
    
    // ignore directional change by 180 degrees (no effect for non-moving snake)
    snakeDirection + 2 == previousDirection && previousDirection != 0 ? snakeDirection = previousDirection : 0;
    snakeDirection - 2 == previousDirection && previousDirection != 0 ? snakeDirection = previousDirection : 0;
  }
}

void initGame() {
  gameOver = false;
  
  //draw boundary
  TV.draw_rect(0, 0, 110, 90, WHITE);
  
  //init snake
  snakeLength = 1;
  memset(snakeSegmentsX, 0, sizeof(maxSnakeLength));
  memset(snakeSegmentsY, 0, sizeof(maxSnakeLength));
  snake.col = random(width);
  snake.row = random(height);
  snakeSegmentsX[0] = snake.col;
  snakeSegmentsY[0] = snake.row;
  TV.draw_rect(snake.col*5, snake.row*5, 5, 5, WHITE, INVERT);
  
  //init food
  food.col = random(width);
  food.row = random(height);
  TV.draw_rect(food.col*5+1, food.row*5+1, 3, 3, WHITE, INVERT);
}

void gameLoop() {

  while (!gameOver) {
    TV.draw_rect(0, 0, 110, 90, WHITE);
  
    //////////////////////////////////////////////////////////////////////////////////////////////
    // if there is no food, generate one
    //////////////////////////////////////////////////////////////////////////////////////////////
    if (food.row == -1 || food.col == -1) {
      food.col = random(width);
      food.row = random(height);

      while (checkCollision(food)) {
        food.col = random(width);
        food.row = random(height);
      }

      TV.draw_rect(food.col*5+1, food.row*5+1, 3, 3, WHITE, INVERT);
    }
  
    //////////////////////////////////////////////////////////////////////////////////////////////
    // calculate snake movement data
    //////////////////////////////////////////////////////////////////////////////////////////////
    switch (snakeDirection) {
    case up:
      snake.row--;
      break;
  
    case right:
      snake.col++;
      break;
  
    case down:
      snake.row++;
      break;
  
    case left:
      snake.col--;
      break;
    }
    
    fixEdge();
    gameOver = checkCollision(snake);
  
    snakeSegmentsX[snakeLength] = snake.col;
    snakeSegmentsY[snakeLength] = snake.row;
  
    
    // check if the food was eaten
    if (snake.row != food.row || snake.col != food.col) {
      //erase snake tail
      TV.draw_rect(snakeSegmentsX[0]*5, snakeSegmentsY[0]*5, 5, 5, BLACK, INVERT);
  
      memmove(&snakeSegmentsX[0], &snakeSegmentsX[1], sizeof(snakeSegmentsX));
      memmove(&snakeSegmentsY[0], &snakeSegmentsY[1], sizeof(snakeSegmentsY));
      TV.draw_rect(snake.col*5, snake.row*5, 5, 5, WHITE, INVERT);
  
    } else {
      food.col = -1;
      food.row = -1;
      snakeLength++;

      //erase food
      TV.draw_rect(food.col*5+1, food.row*5+1, 3, 3, BLACK, INVERT); 
      
    }
    
    //watch joystick and potentiometer movement
    handleInput();
  }
}
