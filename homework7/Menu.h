#ifndef MENU_H
#define MENU_H

#include "LiquidCrystal.h"
#include "Arduino.h"
#include "JoystickManager.h"
#include "EEPROMHandler.h"

class Menu {
private:
  LiquidCrystal& lcd;
  JoystickManager& joystickManager;
  EEPROMHandler& eepromHandler;

  int oldBombsUsed = -1;
  int oldElapsedTime = -1;

  const int minLcdBrightness = 0;
  const int maxLcdBrightness = 200;
  const int minMatrixBrightness = 0;
  const int maxMatrixBrightness = 15;
  const int lcdUpdateStep = 5;
  const int matrixUpdateStep = 1;

  void displayTextOnLCD(const char* text, int textStartIndex, int col, int line);
  void displayCurrentMenuOptions();
  void handleJoystickMenuPress();
  void resetMenuOptions();
  void handleJoystickMenuChange();
  void handleJoystickBrightnessUpdates(int updateValue);
  void addBrightnessValueOnDisplay();
public:
  unsigned long greetingsShownTime = 0;
  const int displayIntroMessageTime = 1000;  // TODO: update me
  bool displayMenu = false;
  bool displayGreetings = true;
  bool displayIntro = true;
  bool displayBomb = false;
  bool displayMenuOptions = false;
  int currMenuBombCol = 0;
  int currMenuBombRow = 0;
  int currMenuOption = 0;
  int currMenuSubOption = -1;
  const int bombCharIndex = 0;
  unsigned long lastMenuPress = 0;
  bool canEnterBrightness = false;
  bool canStartGame = false;

  int currMenuHeight = 4;  // TODO: change this for the settings tab
  const char* menuOptions[4] = {
    "Start", "About", "Settings", "High Score"
  };
  const char* menuAboutSection[4] = {
    "Bomberman",
    "By Andrei Hodoroaga",
    "https://github.com/andreihodoroaga/introductiontorobotics",
    "Back"
  };
  const char* menuSettingsSection[2] = {
    "Brightness",
    "Back"
  };
  const char* brightnessSettings[3] = {
    "LCD",
    "Matrix",
    "Back"
  };
  int aboutSectionNameStartIndex = 0;
  int aboutSectionGitLinkStartIndex = 0;

  Menu(LiquidCrystal& lcdObj, JoystickManager& joystickManagerObj, EEPROMHandler& eepromHandlerObj)
    : lcd(lcdObj), joystickManager(joystickManagerObj), eepromHandler(eepromHandlerObj) {}
  void displayGreetingsOnLCD();
  void displayLCDMenu();
  void displayGameInfo(int bombsUsed, unsigned long elapsedTime);
  void displayEndGameMessage(const char* text);
};

#endif
