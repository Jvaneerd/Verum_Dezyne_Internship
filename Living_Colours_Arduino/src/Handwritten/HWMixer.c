#include "HWMixer.h"
#include "Arduino.h"

#define MIX_POT_PIN A0

typedef enum { Red, Green, Blue } Colours;
int colours[3] = { 0, 0, 0 };
uint8_t currentColour = Red;
bool hwMixer_Started = false;

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void hwMixer_Start(IHWMixer *self) {
  hwMixer_Started = true;
}

void hwMixer_Stop(IHWMixer *self) {
  hwMixer_Started = false;
}

void hwMixer_mixRed(IHWMixer *self) {
  currentColour = Red;
}

void hwMixer_mixGreen(IHWMixer *self) {
  currentColour = Green;
}

void hwMixer_mixBlue(IHWMixer *self) {
  currentColour = Blue;
}

bool hwMixer_checkChanges() {
  int potValue = analogRead(MIX_POT_PIN);
  int mappedPotValue = map(potValue, 0, 1023, 0, 255);

  if (colours[currentColour] != mappedPotValue) {
    colours[currentColour] = mappedPotValue;
    return true;
  } else {
    return false;
  }
}
