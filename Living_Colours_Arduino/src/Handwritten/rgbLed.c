#include "rgbLed.h"

void rgb_SetRed(IRGBLed* self) {
  Colour Red = { .red = 255, .green = 0, .blue = 0 };
  rgb_SetColour(self, Red);
}

void rgb_SetGreen(IRGBLed* self) {
  Colour Green = { .red = 0, .green = 255, .blue = 0 };
  rgb_SetColour(self, Green);
}

void rgb_SetBlue(IRGBLed* self) {
  Colour Blue = { .red = 0, .green = 0, .blue = 255 };
  rgb_SetColour(self, Blue);
}

void rgb_Dim(IRGBLed* self) {
  Colour dim = { .red = 0, .green = 0, .blue = 0 };
  rgb_SetColour(self, dim);
}

void rgb_SetSpecificRed(IRGBLed* self, int value) {
  analogWrite(PIN_RED, value);
}

void rgb_SetSpecificGreen(IRGBLed* self, int value) {
  analogWrite(PIN_GREEN, value);
}

void rgb_SetSpecificBlue(IRGBLed* self, int value) {
  analogWrite(PIN_BLUE, value);
}

void rgb_SetColour(IRGBLed* self, Colour colour) {
  analogWrite(PIN_RED, colour.red);
  analogWrite(PIN_GREEN, colour.green);
  analogWrite(PIN_BLUE, colour.blue);
}
