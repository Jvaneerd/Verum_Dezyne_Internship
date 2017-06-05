#ifndef RGBLED_H
#define RGBLED_H

#include "Arduino.h"
#include "../Generated/IRGBLed.h"

#define PIN_RED 11
#define PIN_GREEN 6
#define PIN_BLUE 5

void rgb_SetRed(IRGBLed* self);
void rgb_SetGreen(IRGBLed* self);
void rgb_SetBlue(IRGBLed* self);

void rgb_Dim(IRGBLed* self);
void rgb_SetColour(IRGBLed* self, Colour colour);

#endif //RGBLED_H
