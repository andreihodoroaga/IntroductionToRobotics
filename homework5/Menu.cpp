#include "Menu.h"
#include "Option.h"
#include "ActionHandler.h"
#include <Arduino.h>  

void Menu::showOptions() {
  Serial.println("Menu");
  for (int i = 0; i < numberOfOptions; i++) {
    Serial.print(i + 1);
    Serial.print(". ");
    Serial.println(options[i].name);
  }
}

void Menu::showSuboptions() {
  if (currentOption == -1) {
    return;
  }

  for (int i = 0; i < options[currentOption].numberOfSubOptions; i++) {
    Serial.print("\t");
    Serial.print(i + 1);
    Serial.print(". ");
    Serial.println(options[currentOption].subOptions[i]);
  }
}

void Menu::parseInput() {
  if (!Serial.available()) {
    return;
  }

  int userInput = Serial.parseInt();

  if (userInput <= 0) {
    return;
  }

  if (state == 1) {
    if (userInput > numberOfOptions) {
      showError();
      state = 0;
      return;
    }
    handleOptionChosen(userInput);
  } else if (state == 3) {
    if (userInput > options[currentOption].numberOfSubOptions) {
      showError();
      state = 2;
      return;
    }
    handleSubOptionChosen(userInput);
  }
}

void Menu::show() {
  switch (state) {
    case 0:
      currentOption = -1;
      currentSubOption = -1;
      state = 1;
      showOptions();
      break;
    case 1:
      parseInput();
      break;
    case 2:
      showSuboptions();
      state = 3;
      break;
    case 3:
      parseInput();
      break;
    default:
      break;
  }
}

void Menu::showError() {
  Serial.println("Please input a valid choice!");
}

void Menu::handleOptionChosen(int option) {
  if (option == 2) {
    Serial.println("Are you sure you want to reset all saved data?");
  }
  currentOption = option - 1;
  state = 2;
} 

void Menu::handleSubOptionChosen(int option) {
  // the last option always takes the user back
  if (option == options[currentOption].numberOfSubOptions) {
    state = 0;
  } else {
    currentSubOption = option - 1;
  }
}
