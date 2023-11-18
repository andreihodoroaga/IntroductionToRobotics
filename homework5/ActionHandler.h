#ifndef ACTION_HANDLER_H
#define ACTION_HANDLER_H

class ActionHandler {
public:
  void handleAction(int option, int subOption);

private:
  enum Color { RED, GREEN, BLUE };

  void changeSensorsSamplingInterval();
  void changeUltrasonicAlertThreshold();
  void changePhotocellAlertThreshold();
  void resetLoggerData();
  void getRGBColors();
  void toggleAutomaticMode();
  void getColor(Color color);
};

#endif