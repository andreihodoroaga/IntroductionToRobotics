#include "Option.h"
#include <Arduino.h>

Option::Option(const char* name, const int numberOfSubOptions, const char** subOptions)
  : numberOfSubOptions(numberOfSubOptions) {

  this->name = strdup(name);

  this->subOptions = new char*[numberOfSubOptions];
  for (int i = 0; i < numberOfSubOptions; ++i) {
    this->subOptions[i] = strdup(subOptions[i]);
  }
}

Option::~Option() {
  free(name);

  for (int i = 0; i < numberOfSubOptions; ++i) {
    free(subOptions[i]);
  }

  delete[] subOptions;
}
