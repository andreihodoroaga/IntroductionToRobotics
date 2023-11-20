#ifndef EEPROM_VARIABLES_H
#define EEPROM_VARIABLES_H

class EEPROMVariables {
public:
  static const int sensorsSamplingIntervalIndex;
  static const int ultrasonicAlertThresholdIndex;
  static const int photocellAlertThresholdIndex;
  static const int rgbLedRedIndex;
  static const int rgbLedGreenIndex;
  static const int rgbLedBlueIndex;
  static const int automaticModeIndex;
  static const int ultrasonicReadingsStartIndex;
  static const int photocellReadingsStartIndex;
};

#endif
