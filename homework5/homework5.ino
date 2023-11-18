#include "Menu.h"
#include "EEPROM.h"

// RGB led
const int rgbLedRedPin = 2;
const int rgbLedGreenPin = 3;
const int rgbLedBluePin = 4;
byte automaticMode;
int storedRedValue;
int storedGreenValue;
int storedBlueValue;

// Ultrasonic sensor
const int ultrasonicTrigPin = 9;
const int ultrasonicEchoPin = 10;
long duration;
int distance;

// Photocell
int photocellPin = 0;

// App variables
Menu menu;
ActionHandler actionHandler;
int sensorSamplingsInterval = 0;
unsigned long lastSensorReadingTime = 0;
unsigned long lastSensorCurrentReadingTime = 0;

int ultrasonicAlertThreshold = 0;
int photocellAlertThreshold = 0;
bool alert = false;

void setup() {
  pinMode(rgbLedRedPin, OUTPUT);
  pinMode(rgbLedGreenPin, OUTPUT);
  pinMode(rgbLedBluePin, OUTPUT);

  pinMode(ultrasonicTrigPin, OUTPUT);
  pinMode(ultrasonicEchoPin, INPUT);

  Serial.begin(9600);
  Serial.println();
}

void loop() {
  menu.show();

  getStoredVariables();
  readSensors();
  handleRGBLedLogic();
  handleMenuActions();
}

void handleMenuActions() {
  if (menu.currentOption == -1 || menu.currentSubOption == -1) {
    return;
  }

  actionHandler.handleAction(menu.currentOption, menu.currentSubOption);

  if (menu.currentOption == 2 && menu.currentSubOption == 0) {
    displayCurrentSensorReadings();
  }
 
  if (menu.currentOption == 2 && menu.currentSubOption == 1) {
    displayCurrentSensorSettings();
  }

  if (menu.currentOption == 2 && menu.currentSubOption == 2) {
    displayLoggedData();
  }

  menu.state = 0;
}

void getStoredVariables() {
  EEPROM.get(0, sensorSamplingsInterval);
  EEPROM.get(2, ultrasonicAlertThreshold);
  EEPROM.get(4, photocellAlertThreshold);
  EEPROM.get(5, storedRedValue);
  EEPROM.get(6, storedGreenValue);
  EEPROM.get(7, storedBlueValue);
  EEPROM.get(8, automaticMode);
}

void displayCurrentSensorReadings() {
  if (sensorSamplingsInterval == 0) {
    Serial.println("First change the sampling interval!");
    return;
  }
  Serial.println("Input any key to exit");
  while (!Serial.available()) {
    if (millis() - lastSensorCurrentReadingTime > 1000 * sensorSamplingsInterval) {
      Serial.print("Photocell: ");
      Serial.println(getPhotocellReading());
      Serial.print("Ultrasonic: ");
      Serial.println(getUltrasonicSensorReading());
      lastSensorCurrentReadingTime = millis();
    }
  }
  menu.state = 0;
}

void displayCurrentSensorSettings() {
  Serial.print("Sampling interval: ");
  Serial.println(sensorSamplingsInterval);
  Serial.print("Ultrasonic alert threshold: ");
  Serial.println(ultrasonicAlertThreshold);
  Serial.print("Photocell alert threshold: ");
  Serial.println(photocellAlertThreshold);
}

void displayLoggedData() {
  Serial.println("Logged ultrasonic readings: ");
  displayReadings(10, 10);

  Serial.println("Logged photocell readings: ");
  displayReadings(30, 10);
}

void readSensors() {
  if (sensorSamplingsInterval == 0) {
    return;
  }
  if (millis() - lastSensorReadingTime > 1000 * sensorSamplingsInterval) {
    // read from sensors
    int newUltrasonicValue = getUltrasonicSensorReading();
    addReadingToEEPROM(10, 10, newUltrasonicValue);

    int newPhotocellValue = getPhotocellReading();
    addReadingToEEPROM(30, 10, newPhotocellValue);

    // if there is an object too close to the ultrasonic or if there it gets too dark
    alert = newUltrasonicValue < ultrasonicAlertThreshold || newPhotocellValue < photocellAlertThreshold;

    lastSensorReadingTime = millis();
  }
}

int getPhotocellReading() {
  return analogRead(photocellPin);
}

int getUltrasonicSensorReading() {
  digitalWrite(ultrasonicTrigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(ultrasonicTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultrasonicTrigPin, LOW);

  duration = pulseIn(ultrasonicEchoPin, HIGH);

  distance = duration * 0.034 / 2;

  return distance;
}

void addReadingToEEPROM(int startIndex, int readingsSize, int newValue) {
  int lastIndex = -1;
  for (int i = startIndex; i < startIndex + readingsSize * 2; i += 2) {
    int currentReading;
    EEPROM.get(i, currentReading);
    if (currentReading == 0) {
      lastIndex = i;
      break;
    }
  }

  if (lastIndex != -1) {
    EEPROM.update(lastIndex, newValue);
    return;
  }

  // add the new reading to the last index in EEPROM
  int lastAvailableIndex = startIndex + (readingsSize - 1) * 2;
  for (int i = startIndex; i < lastAvailableIndex; i += 2) {
    int nextReading;
    EEPROM.get(i + 2, nextReading);
    EEPROM.update(i, nextReading);
  }
  EEPROM.update(lastAvailableIndex, newValue);
}

void displayReadings(int startIndex, int readingsSize) {
  for (int i = startIndex; i < startIndex + readingsSize * 2; i += 2) {
    int currentReading;
    EEPROM.get(i, currentReading);
    if (currentReading == -1) {
      return;
    }
    Serial.print((i - startIndex) / 2 + 1);
    Serial.print(". ");
    Serial.println(currentReading);
  }
}

// https://littlebirdelectronics.com.au/guides/105/how-to-use-an-rgb-led-with-arduino
void setRgbLedColor(int red, int green, int blue) {
  analogWrite(rgbLedRedPin, red);
  analogWrite(rgbLedGreenPin, green);
  analogWrite(rgbLedBluePin, blue);
}

void handleRGBLedLogic() {
  if (!automaticMode) {
    setRgbLedColor(storedRedValue, storedGreenValue, storedBlueValue);
    return;
  }
  if (alert) {
    setRgbLedColor(255, 0, 0);
  } else {
    setRgbLedColor(0, 255, 0);
  }
}
