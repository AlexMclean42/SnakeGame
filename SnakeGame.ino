#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 8, 9, 10, 11, 12); // Adjust pin numbers for your setup

const int joystickXPin = A0; // X-axis of the joystick
const int joystickYPin = A1; // Y-axis of the joystick
const int buttonPin = 6;     // Digital pin for the joystick button

int snakeX, snakeY;
int dirX, dirY;
int foodX, foodY;
int tailX[255], tailY[255];
int tailLength;
int score;
bool gameover;

void setup() {
  lcd.begin(16, 2); // Initialize the LCD
  pinMode(buttonPin, INPUT_PULLUP); // Set button pin as INPUT with internal pull-up resistor
  randomSeed(analogRead(0)); // Initialize random seed

  // Initialize game variables
  snakeX = 0; // Start snake at the top-left corner
  snakeY = 0;
  dirX = 1;
  dirY = 0;
  tailLength = 0;
  score = 0;
  gameover = false;
  spawnFood();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Snake Game");
  delay(1000);
}

void spawnFood() {
  do {
    foodX = random(0, 16);
    foodY = random(0, 2);
  } while (foodX == snakeX && foodY == snakeY);

  // Print the food on the LCD
  lcd.setCursor(foodX, foodY);
  lcd.print("F");
}

void loop() {
  if (!gameover) {
    // Read joystick input
    int joystickXValue = analogRead(joystickXPin);
    int joystickYValue = analogRead(joystickYPin);

    // Update snake direction based on joystick input
    if (joystickXValue < 300) {
      dirX = -1;
      dirY = 0;
    } else if (joystickXValue > 700) {
      dirX = 1;
      dirY = 0;
    } else if (joystickYValue < 300) {
      dirX = 0;
      dirY = -1;
    } else if (joystickYValue > 700) {
      dirX = 0;
      dirY = 1;
    }

    // Store tail positions
    for (int i = tailLength - 1; i > 0; i--) {
      tailX[i] = tailX[i - 1];
      tailY[i] = tailY[i - 1];
    }
    tailX[0] = snakeX;
    tailY[0] = snakeY;

    // Move the snake
    snakeX += dirX;
    snakeY += dirY;

    // Wrap snake position around the display
    if (snakeX < 0) snakeX = 15;
    else if (snakeX > 15) snakeX = 0;
    if (snakeY < 0) snakeY = 1;
    else if (snakeY > 1) snakeY = 0;

    // Check for collisions with food
    if (snakeX == foodX && snakeY == foodY) {
      // Snake ate the food
      tailLength++;
      spawnFood();
      score++;
    }

    // Check for collisions with the tail
    for (int i = 0; i < tailLength; i++) {
      if (snakeX == tailX[i] && snakeY == tailY[i]) {
        gameover = true;
        break;
      }
    }

    // Clear the LCD and draw the snake and food
    lcd.clear();
    lcd.setCursor(snakeX, snakeY);
    lcd.print("o");
    lcd.setCursor(foodX, foodY);
    lcd.print("F");
    for (int i = 0; i < tailLength; i++) {
      lcd.setCursor(tailX[i], tailY[i]);
      lcd.print(".");
    }
    lcd.setCursor(0, 1);
//    lcd.print("Score: ");
//    lcd.print(score);
    delay(300);
  } else {
    // Game over message
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Game Over");
    lcd.setCursor(0, 1);
    lcd.print("Score: ");
    lcd.print(score);
    delay(3000);
    gameover = false;
    setup(); // Restart the game
  }
}
