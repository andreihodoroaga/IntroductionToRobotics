const int buttonPinFloor0 = 2;
const int buttonPinFloor1 = 4;
const int buttonPinFloor2 = 6;
const int operationalElevatorLedPin = 8;
const int floor0LedPin = 10;
const int floor1LedPin = 11;
const int floor2LedPin = 12;

byte buttonStateFloor0 = LOW;
byte buttonStateFloor1 = LOW;
byte buttonStateFloor2 = LOW;
byte lastButtonStateFloor0 = LOW;
byte lastButtonStateFloor1 = LOW;
byte lastButtonStateFloor2 = LOW;

const int buttonDebounceDelay = 75;
unsigned long lastDebounceTimeFloor0 = 0;
unsigned long lastDebounceTimeFloor1 = 0;
unsigned long lastDebounceTimeFloor2 = 0;

byte operationalElevatorLedState = LOW;
unsigned long previousTimeForElevatorLed = 0;

unsigned long timeElevatorButtonPressed = 0;
unsigned long timeElevatorStartedMoving = 0;
byte isElevatorMoving = 0;
int currentFloor = 0;
int floorsTraveled = 0;
int destination = 0;

const int movingBetweenTwoFloorsDuration = 3000;
const int durationPerFloor = 1000;
const int operationalLedBlinkDuration = 500;
const int closingElevatorDoorsDuration = 1500;

byte floor0LedState = LOW;
byte floor1LedState = LOW;
byte floor2LedState = LOW;

const int floor0 = 0;
const int floor1 = 1;
const int floor2 = 2;

unsigned long currentMillis = 0;

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
  currentMillis = millis();

  buttonStateFloor0 = getButtonState(buttonPinFloor0, buttonStateFloor0, lastButtonStateFloor0, lastDebounceTimeFloor0);
  buttonStateFloor1 = getButtonState(buttonPinFloor1, buttonStateFloor1, lastButtonStateFloor1, lastDebounceTimeFloor1);
  buttonStateFloor2 = getButtonState(buttonPinFloor2, buttonStateFloor2, lastButtonStateFloor2, lastDebounceTimeFloor2);

  controlElevator();

  if (!isElevatorMoving) {
    updateLedAtFloor(currentFloor, HIGH);
    handleButtonInput();
  }
}

void handleButtonInput() {
  if (buttonStateFloor0 == HIGH && currentFloor != floor0) {
    moveElevatorToFloor(floor0);
  } 
  if (buttonStateFloor1 == HIGH && currentFloor != floor1) {
    moveElevatorToFloor(floor1);
  }
  if (buttonStateFloor2 == HIGH && currentFloor != floor2) {
    moveElevatorToFloor(floor2);
  }  
}

void moveElevatorToFloor(int destinationFloor) {
  timeElevatorButtonPressed = currentMillis;
  destination = destinationFloor;
  isElevatorMoving = 1;
}

void controlElevator() {
  if (isElevatorMoving) {
    if (currentMillis - timeElevatorButtonPressed < closingElevatorDoorsDuration) {
      updateLedAtFloor(currentFloor, HIGH);
      return;
    } else {
      updateLedAtFloor(currentFloor, LOW);
      timeElevatorStartedMoving = timeElevatorButtonPressed + closingElevatorDoorsDuration;
    }

    blinkLed(operationalElevatorLedPin, operationalElevatorLedState, previousTimeForElevatorLed, operationalLedBlinkDuration);

    floorsTraveled = abs(destination - currentFloor);
    if (floorsTraveled == 2) {
      if (currentMillis > timeElevatorStartedMoving + movingBetweenTwoFloorsDuration &&
          currentMillis < timeElevatorStartedMoving + movingBetweenTwoFloorsDuration + durationPerFloor
      ) {
        updateLedAtFloor(floor1, HIGH);
      }
      else {
        updateLedAtFloor(floor1, LOW);
      }
    }

    if (currentMillis > timeElevatorStartedMoving + movingBetweenTwoFloorsDuration * floorsTraveled) {
      isElevatorMoving = 0;
      currentFloor = destination;
    }
    return;
  }
  
  digitalWrite(operationalElevatorLedPin, LOW);
}

void updateLedAtFloor(int floor, byte newState) {
  if (floor == floor0) {
    digitalWrite(floor0LedPin, newState);
  } else if (floor == floor1) {
    digitalWrite(floor1LedPin, newState);
  } else {
    digitalWrite(floor2LedPin, newState);
  }
}

int getButtonState(int buttonPin, byte oldButtonState, byte& lastButtonState, unsigned long& lastDebounceTime) {
  int newButtonState = oldButtonState;
  int reading = !digitalRead(buttonPin);

  if (reading != lastButtonState) {
    lastDebounceTime = currentMillis;
  }

  if (currentMillis - lastDebounceTime > buttonDebounceDelay) {
    if (reading != oldButtonState) {
      newButtonState = reading;
    }
  }
  lastButtonState = reading;

  return newButtonState;
}

void blinkLed(int ledPin, byte& ledState, unsigned long& previousTime, int blinkDuration) {
  if (currentMillis - previousTime >= blinkDuration) {
    previousTime = currentMillis;

    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    digitalWrite(ledPin, ledState);
  }
}