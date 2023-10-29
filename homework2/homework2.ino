const int buttonPinFloor0 = 2;
const int buttonPinFloor1 = 4;
const int buttonPinFloor2 = 6;

byte buttonStateFloor0 = 0;
byte buttonStateFloor1 = 0;
byte buttonStateFloor2 = 0;
byte lastButtonStateFloor0 = 0;
byte lastButtonStateFloor1 = 0;
byte lastButtonStateFloor2 = 0;

const int buttonDebounceDelay = 75;
unsigned long lastDebounceTimeFloor0 = 0;
unsigned long lastDebounceTimeFloor1 = 0;
unsigned long lastDebounceTimeFloor2 = 0;

const int operationalElevatorLedPin = 8;
byte operationalElevatorLedState = LOW;
unsigned long previousTimeForElevatorLed = 0;

unsigned long timeElevatorStartedMoving = 0;
byte isElevatorMoving = 0;
int currentFloor = 0;
int floorsTraveled = 0;

const int floor0LedPin = 10;
const int floor1LedPin = 11;
const int floor2LedPin = 12;

byte floor0LedState = LOW;
byte floor1LedState = LOW;
byte floor2LedState = LOW;

void setup() {
  Serial.begin(9600);
  pinMode(buttonPinFloor0, INPUT_PULLUP);
  pinMode(buttonPinFloor1, INPUT_PULLUP);
  pinMode(buttonPinFloor2, INPUT_PULLUP);
  pinMode(operationalElevatorLedPin, OUTPUT);
  pinMode(floor0LedPin, OUTPUT);
  pinMode(floor1LedPin, OUTPUT);
  pinMode(floor2LedPin, OUTPUT);
}

void loop() {
  buttonStateFloor0 = getButtonState(buttonPinFloor0, buttonStateFloor0, lastButtonStateFloor0, lastDebounceTimeFloor0);
  buttonStateFloor1 = getButtonState(buttonPinFloor1, buttonStateFloor1, lastButtonStateFloor1, lastDebounceTimeFloor1);
  buttonStateFloor2 = getButtonState(buttonPinFloor2, buttonStateFloor2, lastButtonStateFloor2, lastDebounceTimeFloor2);

  if (isElevatorMoving) {
    blinkLed(operationalElevatorLedPin, operationalElevatorLedState, previousTimeForElevatorLed, 500);
    if (timeElevatorStartedMoving != 0 && millis() > timeElevatorStartedMoving + 3000 * floorsTraveled) {
      isElevatorMoving = 0;
    }
    return;
  }

  digitalWrite(operationalElevatorLedPin, LOW);

  if (currentFloor == 0) {
    digitalWrite(floor0LedPin, HIGH);
  } else if (currentFloor == 1) {
    digitalWrite(floor1LedPin, HIGH);
  } else {
    digitalWrite(floor2LedPin, HIGH);
  }

  if (buttonStateFloor0 == 1 && currentFloor != 0) {
    moveElevator(currentFloor, 0);
  } 
  if (buttonStateFloor1 == 1 && currentFloor != 1) {
    moveElevator(currentFloor, 1);
  }
  if (buttonStateFloor2 == 1 && currentFloor != 2) {
    moveElevator(currentFloor, 2);
  }  
}

void moveElevator(int source, int dest) {
  if (isElevatorMoving) {
    return;
  }

  floorsTraveled = abs(dest - source);

  if (source == 0) {
    digitalWrite(floor0LedPin, LOW);
  } else if (source == 1) {
    digitalWrite(floor1LedPin, LOW);
  } else {
    digitalWrite(floor2LedPin, LOW);
  }

  timeElevatorStartedMoving = millis();
  isElevatorMoving = 1;
  currentFloor = dest;
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

void blinkLed(int ledPin, byte& ledState, unsigned long& previousTime , int blinkDuration) {
  unsigned long currentMillis = millis();
  if (currentMillis - previousTime >= blinkDuration) {
    Serial.println(currentMillis);
    // save the last time you blinked the LED
    previousTime = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
  }
}