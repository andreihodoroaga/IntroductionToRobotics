#include "EEPROMHandler.h"
#include "Arduino.h"

int EEPROMHandler::getLcdBrightness() {
  int brightness;
  EEPROM.get(lcdBrightnessIndex, brightness);
  return brightness;
}

int EEPROMHandler::getMatrixBrightness() {
  int brightness;
  EEPROM.get(matrixBrightnessIndex, brightness);
  return brightness;
}

void EEPROMHandler::updateLCDBrightness(int updateValue, int minValue, int maxValue) {
  updateBrightness(updateValue, minValue, maxValue, getLcdBrightness(), lcdBrightnessIndex);
}

void EEPROMHandler::updateMatrixBrightness(int updateValue, int minValue, int maxValue) {
  updateBrightness(updateValue, minValue, maxValue, getMatrixBrightness(), matrixBrightnessIndex);
}

void EEPROMHandler::updateBrightness(int updateValue, int minValue, int maxValue, int currentValue, int eepromIndex) {
  int value = currentValue + updateValue;
  if (value < minValue || value > maxValue) {
    value = currentValue;
  }
  EEPROM.put(eepromIndex, value);
}
