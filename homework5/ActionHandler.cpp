#include "HardwareSerial.h"
#include <Arduino.h>
#include <EEPROM.h>
#include "ActionHandler.h"

void ActionHandler::handleAction(int option, int subOption) {
  switch (option) {
    case 0:
      switch (subOption) {
        case 0:
          changeSensorsSamplingInterval();
          break;
        case 1:
          changeUltrasonicAlertThreshold();
          break;
        case 2:
          changePhotocellAlertThreshold();
          break;
      }
      break;

    case 1:
      if (subOption == 0) {
        resetLoggerData();
      }
      break;

    case 2:
      break;

    case 3: 
      switch (subOption) {
        case 0:
          getRGBColors();
          break;
        case 1:
          toggleAutomaticMode();
          break;
      }
      break;

    default:
      Serial.println("Unknown option");
      break;
  }
}

void ActionHandler::changeSensorsSamplingInterval() {
  Serial.println("Introduce a value between 1 and 10 (seconds):");

   while (!Serial.available()) {
    // Wait for user input
  }

  int inputValue = Serial.parseInt();
  if (inputValue < 1 || inputValue > 10) {
    Serial.println("Invalid value");
    return;
  }

  EEPROM.put(0, inputValue);
  Serial.println("Sampling interval updated successfully.");
}

void ActionHandler::changeUltrasonicAlertThreshold() {
  Serial.println("Introduce the minimum value where it should be alerting:");

  while (!Serial.available()) { }

  int inputValue = Serial.parseInt();
  EEPROM.put(2, inputValue);
  Serial.println("Ultrasonic alert threshold updated successfully.");
}

void ActionHandler::changePhotocellAlertThreshold() {
  Serial.println("Introduce the minimum value where it should be alerting:");

  while (!Serial.available()) { }

  int inputValue = Serial.parseInt();
  EEPROM.put(4, inputValue);
  Serial.println("Photocell alert threshold updated successfully.");
}

void ActionHandler::resetLoggerData() {
  for (int i = 0; i < EEPROM.length(); ++i) {
    EEPROM.update(i, 0);
  }
  Serial.println("Successfully deleted stored data.");
}

void ActionHandler::getColor(Color color) {
  Serial.println("Introduce a value between 0 and 255");

  while (!Serial.available()) { }

  int inputValue = Serial.parseInt();

  if (inputValue < 0 && inputValue > 255) {
    Serial.println("Invalid value!");
    return;
  }

  Serial.print("Value for ");
  switch (color) {
    case RED:
      EEPROM.update(5, inputValue);
      Serial.print("red");
      break;
    case GREEN:
      EEPROM.update(6, inputValue);
      Serial.print("green");
      break;
    case BLUE:
      EEPROM.update(7, inputValue);
      Serial.print("blue");
      break;
  }
  Serial.print(" updated successfully to ");
  Serial.println(inputValue);
}

void ActionHandler::getRGBColors() {
  getColor(RED);
  getColor(GREEN);
  getColor(BLUE);
}

void ActionHandler::toggleAutomaticMode() {
  byte automaticMode = EEPROM.read(8);
  byte updatedMode = !automaticMode;
  EEPROM.update(8, !automaticMode);
  Serial.print("Automatic mode successfully updated to ");
  Serial.print(updatedMode);
  if (updatedMode) {
    Serial.println(" (ON).");
  } else {
    Serial.println(" (OFF).");
  }
}
