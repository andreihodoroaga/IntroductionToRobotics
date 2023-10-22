/*
  Use the values read from 3 potentiometers to individually control 
  the red, green, and blue channels in an RGB LED.  
*/

const int analogInputPinForRed = A0;
const int analogInputPinForGreen = A2;
const int analogInputPinForBlue = A4;

const int redChannelOutputPin = 3;
const int greenChannelOutputPin = 6;
const int blueChannelOutputPin = 9;

const int minAnalogInput = 0;
const int maxAnalogInput = 1023;
const int minLEDValue = 0;
const int maxLEDValue = 255;

void setup() {
  Serial.begin(9600);

  pinMode(analogInputPinForRed, INPUT);
  pinMode(analogInputPinForGreen, INPUT);
  pinMode(analogInputPinForBlue, INPUT);

  pinMode(redChannelOutputPin, OUTPUT);
  pinMode(greenChannelOutputPin, OUTPUT);
  pinMode(blueChannelOutputPin, OUTPUT);
}

void loop() {
  int analogValueForRed = analogRead(analogInputPinForRed);
  int analogValueForGreen = analogRead(analogInputPinForGreen);
  int analogValueForBlue = analogRead(analogInputPinForBlue);

  int redValue = map(analogValueForRed, minAnalogInput, maxAnalogInput, minLEDValue, maxLEDValue);
  int greenValue = map(analogValueForGreen, minAnalogInput, maxAnalogInput, minLEDValue, maxLEDValue);
  int blueValue = map(analogValueForBlue, minAnalogInput, maxAnalogInput, minLEDValue, maxLEDValue);
    
  analogWrite(redChannelOutputPin, redChannelValue);       
  analogWrite(greenChannelOutputPin, greenChannelValue);       
  analogWrite(blueChannelOutputPin, blueChannelValue);       
}
