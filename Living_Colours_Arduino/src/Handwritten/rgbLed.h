#ifndef RGBLED_H
#define RGBLED_H

#include "Arduino.h"
#include "../Generated/IRGBLed.h"

#define PIN_RED 11
#define PIN_GREEN 6
#define PIN_BLUE 5

void setColor(int red, int green, int blue);

void rgb_setRed(IRGBLed* self);
void rgb_setGreen(IRGBLed* self);
void rgb_setBlue(IRGBLed* self);

void rgb_dim(IRGBLed* self);

void rgb_setSpecificRed(IRGBLed* self, int value);
void rgb_setSpecificGreen(IRGBLed* self, int value);
void rgb_setSpecificBlue(IRGBLed* self, int value);

#endif //RGBLED_H
