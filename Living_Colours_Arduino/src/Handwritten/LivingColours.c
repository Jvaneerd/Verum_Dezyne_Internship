#include "Arduino.h"
#include <assert.h>
#include <dzn/runtime.h>
#include <dzn/locator.h>

#include "../Generated/LivingColours.h"
#include "rgbLed.h"
#include "Timer.h"

void arduinoSetup() {
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
  pinMode(PIN_DEBUG, OUTPUT); // DEFINED IN dzn/runtime.h
  pinMode(PIN_ILLEGAL, OUTPUT); // DEFINED IN dzn/runtime.h
  digitalWrite(PIN_DEBUG, LOW);
  digitalWrite(PIN_ILLEGAL, LOW);
}

int main() {
  runtime dezyne_runtime;
  runtime_init(&dezyne_runtime);

  LivingColours lc;
  LivingColours_init(&lc, 0);

  lc.iRGBLed->in.setRed = rgb_setRed;
  lc.iRGBLed->in.setGreen = rgb_setGreen;
  lc.iRGBLed->in.setBlue = rgb_setBlue;

  lc.iTimer->in.start = startTimer;
  lc.iTimer->in.cancel = clearTimer;

  arduinoSetup();

  lc.iController->in.begin(lc.iController);

  while(1) {  //forever
    if(timeout) {
      lc.iTimer->out.timeout(lc.iTimer);
      timeout = false;
    }
  }

  return 0;
}
