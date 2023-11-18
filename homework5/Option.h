#ifndef OPTION_H
#define OPTION_H

class Option {
public:
  char* name;
  const int numberOfSubOptions;
  char** subOptions;

  Option(const char* name, const int numberOfSubOptions, const char** subOptions);

  ~Option();
};

#endif
