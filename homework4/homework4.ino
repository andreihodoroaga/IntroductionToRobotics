// Button connections
const int startButtonPin = 0;
const int lapButtonPin = 2;
const int resetButtonPin = 3;

byte startButtonState = LOW;
byte lapButtonState = LOW;
byte resetButtonState = LOW;
byte lastStartButtonState = LOW;
byte lastLapButtonState = LOW;
byte lastResetButtonState = LOW;

const int buttonDebounceDelay = 50;
unsigned long lastDebounceTimeStartButton = 0;
unsigned long lastDebounceTimeLapButton = 0;
unsigned long lastDebounceTimeResetButton = 0;

// Shift register connections
const int latchPin = 11;
const int clockPin = 10;
const int dataPin = 12;

const int segD1 = 4;
const int segD2 = 5;
const int segD3 = 6;
const int segD4 = 7;

int displayDigits[] = { segD1, segD2, segD3, segD4 };
const int displayCount = 4;

const int encodingsNumber = 10;
byte byteEncodings[encodingsNumber] = {
  //A B C D E F G DP
  B11111100,  // 0
  B01100000,  // 1
  B11011010,  // 2
  B11110010,  // 3
  B01100110,  // 4
  B10110110,  // 5
  B10111110,  // 6
  B11100000,  // 7
  B11111110,  // 8
  B11110110,  // 9
};

// Timer variables
const int numberOfDigits = 4;
int digits[] = { 0, 0, 0, 0 };
const int digitWithDotIndex = 2;
unsigned long lastTimerUpdate = 0;
const int timerUpdateThreshold = 100;
bool timerRunning = false;
bool prevTimerRunning = false;

byte lastSavedResetButtonState = LOW;
bool isCompletelyReset = true;

const int numberOfLaps = 4;
int laps[numberOfLaps][numberOfDigits] = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };
byte lastSavedLapButtonState = LOW;
int lastSavedLapIdx = -1;
int currentLapOnDisplay = -1;
unsigned long lastLapShownTime = 0;
unsigned long nextLapDebounceTime = 200;
bool lapViewingMode = false;
int numberOfSavedLaps = 0;

void setup() {
  pinMode(startButtonPin, INPUT_PULLUP);
  pinMode(lapButtonPin, INPUT_PULLUP);
  pinMode(resetButtonPin, INPUT_PULLUP);

  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  for (int i = 0; i < displayCount; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }

  Serial.begin(9600);
}

void loop() {
  showDigitsOnDisplay();
  handleButtonsInput();
  manageTimerState();
}

void manageTimerState() {
  if (timerRunning) {
    isCompletelyReset = false;
    startTimer();
  }
}

void startTimer() {
  if (millis() - lastTimerUpdate > timerUpdateThreshold) {
    updateDigitsArr();
    lastTimerUpdate = millis();
  }
}

void handleButtonsInput() {
  startButtonState = getButtonState(startButtonPin, startButtonState, lastStartButtonState, lastDebounceTimeStartButton);
  lapButtonState = getButtonState(lapButtonPin, lapButtonState, lastLapButtonState, lastDebounceTimeLapButton);
  resetButtonState = getButtonState(resetButtonPin, resetButtonState, lastResetButtonState, lastDebounceTimeResetButton);

  handleStartButtonInput();
  handleLapButtonInput();
  handleResetButtonInput();
}

void handleStartButtonInput() {
  if (lapViewingMode) {
    return;
  }

  if (startButtonState == HIGH) {
    timerRunning = !prevTimerRunning;
  } else {
    prevTimerRunning = timerRunning;
  }
}

void handleLapButtonInput() {
  if (lapButtonState == HIGH) {
    if (timerRunning) {
      if (lastSavedLapButtonState == LOW) {
        saveLap();
      }
    } else if (lapViewingMode) {
      showNextLapOnDisplay();
    }
  }
  lastSavedLapButtonState = lapButtonState;
}

void handleResetButtonInput() {
  if (resetButtonState == HIGH) {
    if (!timerRunning && lastSavedResetButtonState == LOW && !isCompletelyReset) {
      resetToInitialState();
      lapViewingMode = !lapViewingMode;

      if (!lapViewingMode) {
        resetLaps();
        isCompletelyReset = true;
      }
    }
  }
  lastSavedResetButtonState = resetButtonState;
}

void resetLaps() {
  numberOfSavedLaps = 0;
  lastSavedLapIdx = -1;

  for (int i = 0; i < numberOfLaps; i++) {
    for (int j = 0; j < numberOfLaps; j++) {
      laps[i][j] = 0;
    }
  }
}

void showNextLapOnDisplay() {
  // If there is not any saved lap yet
  if (lastSavedLapIdx == -1) {
    return;
  }

  if (millis() - lastLapShownTime < nextLapDebounceTime) {
    return;
  }
  lastLapShownTime = millis();

  int numberOfLapsToIterate = numberOfSavedLaps < numberOfLaps ? numberOfSavedLaps : numberOfLaps;
  currentLapOnDisplay = (currentLapOnDisplay + 1) % numberOfLapsToIterate;

  for (int i = 0; i < numberOfDigits; i++) {
    digits[i] = laps[currentLapOnDisplay][i];
  }
}

void saveLap() {
  numberOfSavedLaps++;
  lastSavedLapIdx = (lastSavedLapIdx + 1) % numberOfLaps;

  for (int i = 0; i < numberOfDigits; i++) {
    laps[lastSavedLapIdx][i] = digits[i];
  }
}

void showDigitsOnDisplay() {
  for (int i = 0; i < numberOfDigits; i++) {
    if (i == digitWithDotIndex) {
      writeDigitOnDisplay(digits[i], i, true);
    } else {
      writeDigitOnDisplay(digits[i], i, false);
    }
  }
}

void updateDigitsArr() {
  int carry = 1;
  for (int i = numberOfDigits - 1; i >= 0; i--) {
    digits[i] += carry;
    carry = digits[i] / 10;
    digits[i] %= 10;

    if (carry == 0) {
      break;
    }
  }
}

void resetToInitialState() {
  for (int i = 0; i < numberOfDigits; i++) {
    digits[i] = 0;
  }
}

void writeDigitOnDisplay(int digit, int displayNumber, bool addDot) {
  int encodingToDisplay = byteEncodings[digit];
  if (addDot) {
    encodingToDisplay = addDotToDigitEncoding(encodingToDisplay);
  }

  activateDisplay(displayNumber);
  writeReg(encodingToDisplay);
  writeReg(B00000000);
}

void activateDisplay(int displayNumber) {
  for (int i = 0; i < displayCount; i++) {
    digitalWrite(displayDigits[i], HIGH);
  }
  digitalWrite(displayDigits[displayNumber], LOW);
}

void writeReg(int digit) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, digit);
  digitalWrite(latchPin, HIGH);
}

// make the last bit 1
byte addDotToDigitEncoding(byte encoding) {
  return encoding |= B00000001;
}

int getButtonState(int buttonPin, byte oldButtonState, byte& lastButtonState, unsigned long& lastDebounceTime) {
  int newButtonState = oldButtonState;
  int reading = !digitalRead(buttonPin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if (millis() - lastDebounceTime > buttonDebounceDelay) {
    if (reading != oldButtonState) {
      newButtonState = reading;
    }
  }
  lastButtonState = reading;

  return newButtonState;
}
