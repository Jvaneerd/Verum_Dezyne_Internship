#include <wiringPi.h>
#include "rgbLed.hh"

void rgb_SetColour(Colour colour) {
  softPwmWrite(RED_PIN, colour.red);
  softPwmWrite(GREEN_PIN, colour.green);
  softPwmWrite(BLUE_PIN, colour.blue);
}

void rgb_SetRed() {
  Colour Red = { .red = 100, .green = 0, .blue = 0 };
  rgb_SetColour(Red);
}

void rgb_SetGreen() {
  Colour Green = { .red = 0, .green = 100, .blue = 0 };
  rgb_SetColour(Green);
}

void rgb_SetBlue() {
  Colour Blue = { .red = 0, .green = 0, .blue = 100 };
  rgb_SetColour(Blue);
}

void rgb_Dim() {
  Colour dim = { .red = 0, .green = 0, .blue = 0 };
  rgb_SetColour(dim);
}

void rgb_SetSpecificRed(int value) {
  softPwmWrite(RED_PIN, value);
}

void rgb_SetSpecificGreen(int value) {
  softPwmWrite(GREEN_PIN, value);
}

void rgb_SetSpecificBlue(int value) {
  softPwmWrite(BLUE_PIN, value);
}
