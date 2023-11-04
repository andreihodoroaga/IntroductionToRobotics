// Joystick variables
const int joystickXPin = A0;
const int joystickYPin = A1;
const int joystickSwitchPin = 2;
const int joystickLongPressTimeThreshold = 1500;
const int joystickMaxXValue = 1023;
const int josytickMovementMinThreshold = 400;
const int josytickMovementMaxThreshold = 600;
const bool joystickCommonAnode = false;

int joystickXValue = 0;
int joystickYValue = 0;
byte previousJoystickSwitchState = LOW;
byte joystickSwitchState = LOW;
unsigned long lastJoystickSwitchStateUpdate = 0;
bool joystickLongPressed = false;
bool joystickPressed = false;
bool joystickMoved = false;

// Display variables
const int pinA = 12;
const int pinB = 10;
const int pinC = 9;
const int pinD = 8;
const int pinE = 7;
const int pinF = 6;
const int pinG = 5;
const int pinDP = 4;

const int segSize = 8;
int segments[segSize] = {
  pinDP, pinA, pinB, pinC, pinD, pinE, pinF, pinG
};
int segmentMatrix[segSize][4] = {
  { NULL, NULL, pinC, NULL },   // dp
  { NULL, pinG, pinF, pinB },   // a
  { pinA, pinG, pinF, NULL },   // b
  { pinG, pinD, pinE, pinDP },  // c
  { pinG, NULL, pinE, pinC },   // d
  { pinG, pinD, NULL, pinC },   // e
  { pinA, pinG, NULL, pinB },   // f
  { pinA, pinD, NULL, NULL }    // g
};
int segmentStates[segSize] = {
  LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW
};

const int up = 0;
const int down = 1;
const int left = 2;
const int right = 3;
const int blinkDuration = 300;
int currentSegmentIndex = 0;
unsigned long lastBlinkTime = 0;

void setup() {
  Serial.begin(9600);
  pinMode(joystickSwitchPin, INPUT_PULLUP);
  if (joystickCommonAnode) {
    joystickSwitchState = !joystickSwitchState;
  }

  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i], OUTPUT);
  }
}

void loop() {
  handleJoystickInput();
  updateSegmentsBasedOnJoystickInput();
  renderSegmentsOnDisplay();
  blinkCurrentSegment();
}

void handleJoystickInput() {
  joystickXValue = analogRead(joystickXPin);
  joystickYValue = analogRead(joystickYPin);
  // this is just to make the joystick movements behave as expected in my specific setup
  joystickXValue = joystickMaxXValue - joystickXValue;

  if (joystickXValue >= josytickMovementMinThreshold && joystickXValue <= josytickMovementMaxThreshold
      && joystickYValue >= josytickMovementMinThreshold && joystickYValue <= josytickMovementMaxThreshold) {
    joystickMoved = false;
  }

  handleJoystickPress();
}

void handleJoystickPress() {
  joystickSwitchState = digitalRead(joystickSwitchPin) == LOW ? HIGH : LOW;

  if (joystickSwitchState != previousJoystickSwitchState) {
    previousJoystickSwitchState = joystickSwitchState;
    lastJoystickSwitchStateUpdate = millis();

    if (joystickSwitchState == HIGH) {
      joystickPressed = true;
    }
  }

  joystickLongPressed = false;
  if (joystickSwitchState == HIGH && millis() - lastJoystickSwitchStateUpdate > joystickLongPressTimeThreshold) {
    joystickLongPressed = true;
  }
}

void updateSegmentsBasedOnJoystickInput() {
  flipCurrentSegmentState();
  if (joystickLongPressed) {
    resetDisplay();
  }
  moveCurrentSegment();
}

void flipCurrentSegmentState() {
  if (joystickPressed) {
    segmentStates[currentSegmentIndex] = !segmentStates[currentSegmentIndex];
    joystickPressed = false;
  }
}

void moveCurrentSegment() {
  if (joystickMoved) {
    return;
  }
  if (joystickXValue < josytickMovementMinThreshold) {
    updateCurrentSegmentIndex(left);
  }
  if (joystickXValue > josytickMovementMaxThreshold) {
    updateCurrentSegmentIndex(right);
  }
  if (joystickYValue < josytickMovementMinThreshold) {
    updateCurrentSegmentIndex(down);
  }
  if (joystickYValue > josytickMovementMaxThreshold) {
    updateCurrentSegmentIndex(up);
  }
}

void renderSegmentsOnDisplay() {
  for (int i = 0; i < segSize; i++) {
    if (i != currentSegmentIndex) {
      digitalWrite(segments[i], segmentStates[i]);
    }
  }
}

void blinkCurrentSegment() {
  if (millis() - lastBlinkTime < blinkDuration) {
    return;
  }

  byte currentSegmentState = digitalRead(segments[currentSegmentIndex]);
  digitalWrite(segments[currentSegmentIndex], !currentSegmentState);
  lastBlinkTime = millis();
}

void updateCurrentSegmentIndex(int direction) {
  joystickMoved = true;

  int nextSegment = segmentMatrix[currentSegmentIndex][direction];
  if (!nextSegment) {
    return;
  }

  currentSegmentIndex = getSegmentIndex(nextSegment);
}

int getSegmentIndex(const int segmentPin) {
  for (int i = 0; i < segSize; i++) {
    if (segments[i] == segmentPin) {
      return i;
    }
  }
  return NULL;
}

void resetDisplay() {
  for (int i = 0; i < segSize; i++) {
    segmentStates[i] = LOW;
  }
  currentSegmentIndex = 0;
}
