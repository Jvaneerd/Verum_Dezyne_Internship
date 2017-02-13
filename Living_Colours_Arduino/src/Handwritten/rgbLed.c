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
