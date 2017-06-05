#include "Arduino.h"
#include <assert.h>
#include <dzn/runtime.h>
#include <dzn/locator.h>

#include "rgbLed.h"
#include "../Generated/LivingColoursSystem.h"
#include "Timer.h"
#include "PotMeter.h"

#define PRESETBTN_PIN 12
#define MIXBTN_PIN    13

int preset_btn;
int mix_btn;
int last_preset_btn = LOW;
int last_mix_btn = LOW;

unsigned long last_dbnc_time = 0;
unsigned long dbnc_delay = 50;

void arduinoSetup() {
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
  pinMode(PIN_DEBUG, OUTPUT); // DEFINED IN dzn/runtime.h
  pinMode(PIN_ILLEGAL, OUTPUT); // DEFINED IN dzn/runtime.h
  pinMode(PRESETBTN_PIN, INPUT_PULLUP);
  pinMode(MIXBTN_PIN, INPUT_PULLUP);
  digitalWrite(PIN_DEBUG, LOW);
  digitalWrite(PIN_ILLEGAL, LOW);
}

int main() {
  init();
  arduinoSetup();
  runtime dezyne_runtime;
  runtime_init(&dezyne_runtime);

  LivingColoursSystem lc;
  LivingColoursSystem_init(&lc, 0);

  lc.iRGBLed->in.setRed = rgb_SetRed;
  lc.iRGBLed->in.setGreen = rgb_SetGreen;
  lc.iRGBLed->in.setBlue = rgb_SetBlue;
  lc.iRGBLed->in.dim = rgb_Dim;
  lc.iRGBLed->in.setColour = rgb_SetColour;

  lc.iTimer->in.start = tm_StartTimer;
  lc.iTimer->in.cancel = tm_ClearTimer;

  lc.iPotMeter->in.start = pm_Start;
  lc.iPotMeter->in.stop = pm_Stop;


  lc.iController->in.begin(lc.iController);

  while(1) {  //forever
    if(timeout) {
      lc.iTimer->out.timeout(lc.iTimer);
      timeout = false;
    }

    int preset_btn_pressed = !digitalRead(PRESETBTN_PIN);
    int mix_btn_pressed = !digitalRead(MIXBTN_PIN);

    if (preset_btn_pressed != last_preset_btn) {
      last_dbnc_time = millis();
    }

    if (mix_btn_pressed != last_mix_btn) {
      last_dbnc_time = millis();
    }

    if((millis() - last_dbnc_time) > dbnc_delay) {

      if(preset_btn_pressed != preset_btn) {
        preset_btn = preset_btn_pressed;

        if(preset_btn) {
          lc.iController->in.presetButtonPressed(lc.iController);
        }
      }

      if(mix_btn_pressed != mix_btn) {
        mix_btn = mix_btn_pressed;

        if(mix_btn) {
          lc.iController->in.mixButtonPressed(lc.iController);
        }
      }
    }
    if(pm_Started) {
      if(pm_CheckChanges()) {
        lc.iPotMeter->out.valueChanged(lc.iPotMeter, currentValue);
      }
    }

    last_preset_btn = preset_btn_pressed;
    last_mix_btn = mix_btn_pressed;
  }

  return 0;
}
