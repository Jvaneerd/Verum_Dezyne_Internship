#include "Arduino.h"
#include <assert.h>
#include <dzn/runtime.h>
#include <dzn/locator.h>

#include "../Generated/LivingColours.h"
#include "rgbLed.h"
#include "Timer.h"
#include "PresetSelector.h"
#include "HWMixer.h"

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
  runtime dezyne_runtime;
  runtime_init(&dezyne_runtime);

  LivingColours lc;
  LivingColours_init(&lc, 0);

  lc.iDemoRGBLed->in.setRed = rgb_setRed;
  lc.iDemoRGBLed->in.setGreen = rgb_setGreen;
  lc.iDemoRGBLed->in.setBlue = rgb_setBlue;

  lc.iMixerRGBLed->in.setSpecificRed = rgb_setSpecificRed;
  lc.iMixerRGBLed->in.setSpecificGreen = rgb_setSpecificGreen;
  lc.iMixerRGBLed->in.setSpecificBlue = rgb_setSpecificBlue;

  lc.iTimer->in.start = startTimer;
  lc.iTimer->in.cancel = clearTimer;

  lc.iPresetSelector->in.selectPreset = selectPreset;
  lc.iPresetSelector->in.stop = stopPreset;

  lc.iHWMixer->in.start = hwMixer_Start;
  lc.iHWMixer->in.stop = hwMixer_Stop;
  lc.iHWMixer->in.mixRed = hwMixer_mixRed;
  lc.iHWMixer->in.mixGreen = hwMixer_mixGreen;
  lc.iHWMixer->in.mixBlue = hwMixer_mixBlue;

  init();
  arduinoSetup();

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
    if(hwMixer_Started) {
      if(hwMixer_checkChanges()) {
        lc.iHWMixer->out.valueChanged(lc.iHWMixer, colours[currentColour]);
      }
    }

    last_preset_btn = preset_btn_pressed;
    last_mix_btn = mix_btn_pressed;
  }

  return 0;
}
