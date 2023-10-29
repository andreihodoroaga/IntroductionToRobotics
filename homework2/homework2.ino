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

const int floor0LedPin = 10;
const int floor1LedPin = 11;
const int floor2LedPin = 12;

byte floor0LedState = LOW;
byte floor1LedState = LOW;
byte floor2LedState = LOW;

unsigned long previousTimeLed0 = 0;
unsigned long previousTimeLed1 = 0;
unsigned long previousTimeLed2 = 0;

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

  elevator();

  if (isElevatorMoving) {
    return;
  }

  if (currentFloor == 0) {
    digitalWrite(floor0LedPin, HIGH);
  } else if (currentFloor == 1) {
    digitalWrite(floor1LedPin, HIGH);
  } else {
    digitalWrite(floor2LedPin, HIGH);
  }

  if (buttonStateFloor0 == 1 && currentFloor != 0) {
    moveElevatorToFloor(0);
  } 
  if (buttonStateFloor1 == 1 && currentFloor != 1) {
    moveElevatorToFloor(1);
  }
  if (buttonStateFloor2 == 1 && currentFloor != 2) {
    moveElevatorToFloor(2);
  }  
}

void moveElevatorToFloor(int destinationFloor) {
  if (isElevatorMoving) {
    return;
  }

  timeElevatorButtonPressed = currentMillis;

  destination = destinationFloor;
  isElevatorMoving = 1;
}

void elevator() {
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
      if (timeElevatorStartedMoving != 0 && 
          currentMillis > timeElevatorStartedMoving + movingBetweenTwoFloorsDuration &&
          currentMillis < timeElevatorStartedMoving + movingBetweenTwoFloorsDuration + durationPerFloor
      )
        blinkLed(floor1LedPin, floor1LedState, previousTimeLed1, durationPerFloor);
      else
        digitalWrite(floor1LedPin, 0);
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
  if (floor == 0) {
    digitalWrite(floor0LedPin, newState);
  } else if (floor == 1) {
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