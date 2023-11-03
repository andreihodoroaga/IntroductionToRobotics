const int joystickXPin = A0;
const int joystickYPin = A1;
const int joystickSwitchPin = 2;
const int joystickLongPressThreshold = 1500;
const int joystickMaxXValue = 1023;

int joystickXValue = 0;
int joystickYValue = 0;
byte joystickSwitchState = LOW;
byte previousJoystickSwitchState = LOW;
unsigned long lastJoystickSwitchStateUpdate = 0;
bool joyStickLongPressed = false;

void setup() {
  Serial.begin(9600);
  pinMode(joystickSwitchPin, INPUT_PULLUP);
}

void loop() {
  handleJoystickInput();
}

void handleJoystickInput() {
  joystickXValue = analogRead(joystickXPin);
  joystickYValue = analogRead(joystickYPin);
  joystickSwitchState = digitalRead(joystickSwitchPin) == LOW ? HIGH : LOW;
  // this is just to make the joystick movements behave as expected in the position I chose
  joystickXValue = joystickMaxXValue - joystickXValue;

  if (joystickSwitchState != previousJoystickSwitchState) {
    lastJoystickSwitchStateUpdate = millis();
    previousJoystickSwitchState = joystickSwitchState;
  }

  joyStickLongPressed = false;
  if (joystickSwitchState == HIGH && millis() - lastJoystickSwitchStateUpdate > joystickLongPressThreshold) {
    joyStickLongPressed = true;
  }
}
