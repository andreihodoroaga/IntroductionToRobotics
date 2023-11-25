#include "LedControl.h"

// Joystick
const int joystickXPin = A0;
const int joystickYPin = A1;
const int joystickSwitchPin = 2;
const int joystickLongPressTimeThreshold = 1500;
const int joystickMaxValue = 1023;
const int josytickMovementMinThreshold = 350;
const int josytickMovementMaxThreshold = 650;
const bool joystickCommonAnode = false;
int joystickXValue = 0;
int joystickYValue = 0;
byte previousJoystickSwitchState = LOW;
byte joystickSwitchState = LOW;
unsigned long lastJoystickSwitchStateUpdate = 0;
unsigned long joystickLastLongPressUpdate = 0;
bool joystickLongPressed = false;
bool joystickPressed = false;
bool playerMoved = false;
unsigned long lastJoystickMovement = 0;
const int joystickMovementTimeThreshold = 400;
enum Direction {
  UP,
  DOWN,
  LEFT,
  RIGHT
};
Direction movementDirection;

// Matrix
const byte dinPin = 12;
const byte clockPin = 11;
const byte loadPin = 10;
const byte matrixSize = 8;
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);
byte matrixBrightness = 2;

// Menu
bool showDifficultyOptions = true;
bool gameStarted = false;
int difficulty = 0;
int maxDifficulty = 15;
int lastImgIdx = 0;
unsigned long lastLoadingImageDisplayTime = 0;
const int loadingImageChangeTime = 500;

// Game
const int boardSize = 8;
int board[boardSize][boardSize];
bool shouldResetGame = true;
int playerPositionRow;
int playerPositionCol;
const int empty = 0;
const int wall = 1;
const int bomb = 2;
unsigned long lastPlayerBlinkTime;
const int slowBlinkTime = 500;
unsigned long lastBombBlinkTime = 0;
const int fastBlinkTime = 100;
unsigned long bombPlacementTime = 0;
bool bombPlaced = false;
const int bombExplosionTime = 1600;
unsigned long explodeAnimationTime = 400;
int bombPositionRow = -1;
int bombPositionCol = -1;
bool gameLost;
bool gameWon;

// Animations
const uint64_t loadingStateImages[] = {
  0x0000000000000000,
  0x0000040000000000,
  0x00000c0000000000,
  0x00001c0000000000,
  0x00003c0000000000
};
const int imagesLen = sizeof(loadingStateImages) / 8;
byte smile[8] = { 0x3C, 0x42, 0xA5, 0x81, 0xA5, 0x99, 0x42, 0x3C };
byte sad[8] = { 0x3C, 0x42, 0xA5, 0x81, 0x99, 0xA5, 0x42, 0x3C };

void setup() {
  Serial.begin(9600);

  pinMode(joystickSwitchPin, INPUT_PULLUP);
  if (joystickCommonAnode) {
    joystickSwitchState = !joystickSwitchState;
  }

  lc.shutdown(0, false);
  lc.setIntensity(0, matrixBrightness);
  lc.clearDisplay(0);
}

void loop() {
  if (!gameStarted) {
    chooseDifficulty();
    displayLoadingState();
    return;
  }
  handleJoystickInput();
  resetGame();
  handlePlayerMovement();
  displayBoardOnMatrix();
  handleBombPlacement();
  explodeBomb();
  checkGameWon();
  handleResetGame();
}

void handleBombPlacement() {
  if (!joystickPressed || bombPlaced) {
    return;
  }

  board[playerPositionRow][playerPositionCol] = bomb;
  bombPlacementTime = millis();
  bombPlaced = true;
}

void explodeBomb() {
  if (!bombPlaced || millis() - bombPlacementTime < bombExplosionTime) {
    return;
  }

  unsigned long detonationTime = bombPlacementTime + bombExplosionTime;
  bool animate = millis() - detonationTime < explodeAnimationTime;

  for (int i = 0; i < boardSize; i++) {
    for (int j = 0; j < boardSize; j++) {
      if (board[i][j] == bomb) {
        bombPositionRow = i;
        bombPositionCol = j;
      }
    }
  }
  destroyArea(bombPositionRow, bombPositionCol, animate);
}

// first animate the bomb going off and then destroy the walls
void destroyArea(int row, int col, bool animate) {
  updateNeighborsBomb(row, col, animate);
  if (!animate) {
    bombPlaced = false;
  }
}

void updateNeighborsBomb(int row, int col, bool animate) {
  int newValue = animate ? 1 : 0;
  const int animationLen = 5;
  int directionsDestroyAnimation[animationLen][2] = { { 0, 0 }, { 0, -1 }, { 0, 1 }, { -1, 0 }, { 1, 0 } };

  for (int i = 0; i < animationLen; ++i) {
    int newRow = row + directionsDestroyAnimation[i][0];
    int newCol = col + directionsDestroyAnimation[i][1];
    if (newRow >= 0 && newRow < boardSize && newCol >= 0 && newCol < boardSize) {
      board[newRow][newCol] = newValue;
      if (!animate && playerPositionRow == newRow && playerPositionCol == newCol) {
        gameLost = true;
      }
    }
  }
}

void displayBoardOnMatrix() {
  for (int i = 0; i < boardSize; i++) {
    for (int j = 0; j < boardSize; j++) {
      if (board[i][j] == wall) {
        lc.setLed(0, i, j, true);
      } else if (board[i][j] == bomb) {
        blinkMatrixLed(i, j, lastBombBlinkTime, fastBlinkTime);
      } else if (i == playerPositionRow && j == playerPositionCol) {
        blinkMatrixLed(i, j, lastPlayerBlinkTime, slowBlinkTime);
      } else if (board[i][j] == empty) {
        lc.setLed(0, i, j, false);
      }
    }
  }
}

void handlePlayerMovement() {
  if (!playerMoved) {
    return;
  }
  playerMoved = false;

  switch (movementDirection) {
    case LEFT:
      if (playerPositionCol == 0 || board[playerPositionRow][playerPositionCol - 1]) {
        return;
      }
      playerPositionCol -= 1;
      break;
    case RIGHT:
      if (playerPositionCol == boardSize - 1 || board[playerPositionRow][playerPositionCol + 1]) {
        return;
      }
      playerPositionCol += 1;
      break;
    case DOWN:
      if (playerPositionRow == boardSize - 1 || board[playerPositionRow + 1][playerPositionCol]) {
        return;
      }
      playerPositionRow += 1;
      break;
    case UP:
      if (playerPositionRow == 0 || board[playerPositionRow - 1][playerPositionCol]) {
        return;
      }
      playerPositionRow -= 1;
      break;
  }
}

void checkGameWon() {
  for (int i = 0; i < boardSize; i++) {
    for (int j = 0; j < boardSize; j++) {
      if (board[i][j] == wall) {
        return;
      }
    }
  }
  gameWon = true;
}

void handleResetGame() {
  if (!joystickLongPressed && !gameLost && !gameWon) {
    return;
  }

  if (gameWon) {
    printByte(smile);
    delay(3000);
  } else if (gameLost) {
    printByte(sad);
    delay(2000);
  }
  shouldResetGame = true;
  showDifficultyOptions = true;
  gameStarted = false;
}

void resetGame() {
  if (!shouldResetGame) {
    return;
  }

  shouldResetGame = false;
  gameLost = false;
  gameWon = false;
  bombPlaced = false;
  bombPlacementTime = 0;
  bombPositionRow = -1;
  bombPositionCol = -1;
  playerPositionRow = random(boardSize);
  playerPositionCol = random(boardSize);

  for (int i = 0; i < boardSize; i++) {
    for (int j = 0; j < boardSize; j++) {
      // choosing the 0s and 1s on the board based on the difficulty
      int randomNum = random(maxDifficulty);
      if (randomNum < 3 * difficulty) {
        board[i][j] = 1;
      } else {
        board[i][j] = 0;
      }
    }
  }
  difficulty = 0;

  // leave some empty spaces near the player
  board[playerPositionRow][playerPositionCol] = 0;
  if (playerPositionRow > 0) {
    board[playerPositionRow - 1][playerPositionCol] = 0;
  }
  if (playerPositionRow < boardSize - 1) {
    board[playerPositionRow + 1][playerPositionCol] = 0;
  }
  if (playerPositionCol > 0) {
    board[playerPositionRow][playerPositionCol - 1] = 0;
  }
  if (playerPositionCol < boardSize - 1) {
    board[playerPositionRow][playerPositionCol + 1] = 0;
  }
}

void blinkMatrixLed(int row, int col, unsigned long& lastBlinkTime, int blinkDuration) {
  if (millis() - lastBlinkTime < blinkDuration) {
    return;
  }
  if (millis() - lastBlinkTime < 2 * blinkDuration) {
    lc.setLed(0, row, col, true);
    return;
  }
  lc.setLed(0, row, col, false);
  lastBlinkTime = millis();
}

void chooseDifficulty() {
  if (showDifficultyOptions) {
    Serial.println();
    Serial.println("Choose a difficulty level (input a number):");
    Serial.println("1. Easy");
    Serial.println("2. Medium");
    Serial.println("3. Hard");
    showDifficultyOptions = false;
  }

  if (Serial.available() > 0) {
    int choice = Serial.parseInt();
    switch (choice) {
      case 1:
        Serial.println("You've chosen Easy!");
        difficulty = 1;
        break;
      case 2:
        Serial.println("You've chosen Medium!");
        difficulty = 2;
        break;
      case 3:
        Serial.println("You've chosen Hard!");
        difficulty = 3;
        break;
      default:
        Serial.println("Invalid choice. Please enter 1, 2, or 3.");
        showDifficultyOptions = true;
        break;
    }
    gameStarted = difficulty != 0;
  }
}

void handleJoystickInput() {
  joystickXValue = analogRead(joystickXPin);
  joystickYValue = analogRead(joystickYPin);
  // this is just to make the joystick movements behave as expected in my specific setup
  joystickYValue = joystickMaxValue - joystickYValue;

  if (!(joystickXValue >= josytickMovementMinThreshold && joystickXValue <= josytickMovementMaxThreshold
        && joystickYValue >= josytickMovementMinThreshold && joystickYValue <= josytickMovementMaxThreshold)) {
    if (millis() - lastJoystickMovement > joystickMovementTimeThreshold) {
      playerMoved = true;
      lastJoystickMovement = millis();
      handleJoystickDirectionChange();
    }
  }

  handleJoystickPress();
}

void handleJoystickDirectionChange() {
  if (joystickXValue < josytickMovementMinThreshold) {
    movementDirection = LEFT;
  }
  if (joystickXValue > josytickMovementMaxThreshold) {
    movementDirection = RIGHT;
  }
  if (joystickYValue < josytickMovementMinThreshold) {
    movementDirection = DOWN;
  }
  if (joystickYValue > josytickMovementMaxThreshold) {
    movementDirection = UP;
  }
}

void handleJoystickPress() {
  joystickSwitchState = digitalRead(joystickSwitchPin) == LOW ? HIGH : LOW;

  if (joystickSwitchState != previousJoystickSwitchState) {
    previousJoystickSwitchState = joystickSwitchState;
    lastJoystickSwitchStateUpdate = millis();
    joystickPressed = joystickSwitchState == HIGH;
  }

  joystickLongPressed = false;
  if (joystickSwitchState == HIGH && millis() - lastJoystickSwitchStateUpdate > joystickLongPressTimeThreshold) {
    // i don't want to keep resetting the game over and over when the button is long pressed
    if (millis() - joystickLastLongPressUpdate < joystickLongPressTimeThreshold) {
      return;
    }
    joystickLongPressed = true;
    joystickLastLongPressUpdate = millis();
  }
}

// this function and the emojis are from here:
// https://techatronic.com/facial-expression-on-8x8-led-matrix-using-arduino/
void printByte(byte character[]) {
  int i = 0;
  for (i = 0; i < 8; i++) {
    lc.setRow(0, i, character[i]);
  }
}

void displayLoadingState() {
  if (millis() - lastLoadingImageDisplayTime < loadingImageChangeTime) {
    return;
  }
  displayImage(loadingStateImages[lastImgIdx]);
  if (++lastImgIdx >= imagesLen) {
    lastImgIdx = 0;
  }
  lastLoadingImageDisplayTime = millis();
}

// https://xantorohara.github.io/led-matrix-editor/#
void displayImage(uint64_t image) {
  for (int i = 0; i < 8; i++) {
    byte row = (image >> i * 8) & 0xFF;
    for (int j = 0; j < 8; j++) {
      lc.setLed(0, i, j, bitRead(row, j));
    }
  }
}
