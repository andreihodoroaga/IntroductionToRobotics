#ifndef MENU_H
#define MENU_H

#include "Option.h"  
#include "ActionHandler.h"

class Menu {
private:
  static const int numberOfOptions = 4;

  const char* subOptions[numberOfOptions][4] = {
    { "Sensors Sampling Interval", "Ultrasonic Alert Threshold", "LDR Alert Threshold", "Back" },
    { "Yes", "No" },
    { "Current Sensor Readings", "Current Sensor Settings", "Display Logged Data", "Back" },
    { "Manual Color Control", "LED: Toggle Automatic ON/OFF", "Back" }
  };

  Option options[numberOfOptions] = {
    Option("Sensor Settings", 4, subOptions[0]),
    Option("Reset Logger Data", 2, subOptions[1]),
    Option("System Status", 4, subOptions[2]),
    Option("RGB LED Control", 3, subOptions[3])
  };

public:
  int state = 0;
  int currentOption = -1;
  int currentSubOption = -1;

  ActionHandler actionHandler;

  void show();
  void showOptions();
  void showSuboptions();
  void parseInput();
  void showError();
  void handleOptionChosen(int option);
  void handleSubOptionChosen(int option);
};

#endif
