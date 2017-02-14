#include "Arduino.h"
#include <assert.h>
#include <dzn/runtime.h>
#include <dzn/locator.h>

#include "../Generated/LivingColours.h"
#include "rgbLed.h"
#include "Timer.h"
#include "PresetSelector.h"

#define PRESETBTN_PIN 12

int preset_btn;
int last_preset_btn = LOW;

unsigned long last_dbnc_time = 0;
unsigned long dbnc_delay = 50;

void arduinoSetup() {
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
  pinMode(PIN_DEBUG, OUTPUT); // DEFINED IN dzn/runtime.h
  pinMode(PIN_ILLEGAL, OUTPUT); // DEFINED IN dzn/runtime.h
  pinMode(PRESETBTN_PIN, INPUT_PULLUP);
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
  lc.iPresetSelector->in.selectPreset = selectPreset;
  lc.iPresetSelector->in.stop = stopPreset;
  init();
  arduinoSetup();

  lc.iController->in.begin(lc.iController);

  while(1) {  //forever
    if(timeout) {
      lc.iTimer->out.timeout(lc.iTimer);
      timeout = false;
    }

    int preset_btn_pressed = !digitalRead(PRESETBTN_PIN);
    if (preset_btn_pressed != last_preset_btn) {
      last_dbnc_time = millis();
    }

    if((millis() - last_dbnc_time) > dbnc_delay) {
      if(preset_btn_pressed != preset_btn) {
        preset_btn = preset_btn_pressed;

        if(preset_btn) {
          lc.iController->in.presetButtonPressed(lc.iController);
        }
      }
    }

    last_preset_btn = preset_btn_pressed;
  }

  return 0;
}
