#include "PotMeter.h"
#include "Arduino.h"

#define MIX_POT_PIN A0

int currentValue = 0;
bool pm_Started = false;

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void pm_Start(IPotMeter *self) {
  pm_Started = true;
}

void pm_Stop(IPotMeter *self) {
  pm_Started = false;
}

bool pm_CheckChanges() {
  int potValue = analogRead(MIX_POT_PIN);
  int mappedPotValue = map(potValue, 0, 1023, 0, 255);

  if (currentValue != mappedPotValue) {
    currentValue = mappedPotValue;
    return true;
  } else {
    return false;
  }
}
