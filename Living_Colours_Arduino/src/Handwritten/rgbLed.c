#include "rgbLed.h"

void setColor(int red, int green, int blue) {
  analogWrite(PIN_RED, red);
  analogWrite(PIN_GREEN, green);
  analogWrite(PIN_BLUE, blue);
}

void rgb_setRed(IRGBLed* self) {
  setColor(255, 0, 0);
}

void rgb_setGreen(IRGBLed* self) {
  setColor(0, 255, 0);
}

void rgb_setBlue(IRGBLed* self) {
  setColor(0, 0, 255);
}

void rgb_dim(IRGBLed* self) {
  setColor(0, 0, 0);
}

void rgb_setSpecificRed(IRGBLed* self, int value) {
  analogWrite(PIN_RED, value);
}

void rgb_setSpecificGreen(IRGBLed* self, int value) {
  analogWrite(PIN_GREEN, value);
}

void rgb_setSpecificBlue(IRGBLed* self, int value) {
  analogWrite(PIN_BLUE, value);
}
