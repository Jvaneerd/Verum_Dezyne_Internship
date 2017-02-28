#include <dzn/runtime.hh>
#include <dzn/locator.hh>

#include <wiringPi.h>
#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <cassert>

#include "../Generated/LivingColours.hh"

#include "rgbLed.hh"

#define PRESETBTN_PIN 10
#define MIXBTN_PIN    11

#define DEBOUNCE_TIME 50

volatile bool timeout = false;

using namespace std;

void wiringSetup() {
  wiringPiSetup();
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(PRESETBTN_PIN, INPUT);
  pinMode(MIXBTN_PIN, INPUT);

  softPwmCreate(RED_PIN, 0, 100);
  softPwmCreate(GREEN_PIN, 0, 100);
  softPwmCreate(BLUE_PIN, 0, 100);

  pullUpDnControl(PRESETBTN_PIN, PUD_UP);
  pullUpDnControl(MIXBTN_PIN, PUD_UP);
}

int main(void) {
  wiringSetup();
  signal(SIGALRM, [](int signal){ timeout = true; });

  // Construct the Dezyne runtime
  dzn::locator loc;
  dzn::runtime rt;
  dzn::illegal_handler illegal_handler;
  loc.set(rt);
  loc.set(illegal_handler);

  #ifdef NO_DEBUG
  ostream nullstream(0);
  loc.set(nullstream);
  #endif //NO_DEBUG

  LivingColours lc(loc);
  lc.dzn_meta.name = "Living_Colours_System";

  lc.iRGBLed.in.setRed = rgb_SetRed;
  lc.iRGBLed.in.setGreen = rgb_SetGreen;
  lc.iRGBLed.in.setBlue = rgb_SetBlue;
  lc.iRGBLed.in.dim = rgb_Dim;
  lc.iRGBLed.in.setSpecificRed = rgb_SetSpecificRed;
  lc.iRGBLed.in.setSpecificGreen = rgb_SetSpecificGreen;
  lc.iRGBLed.in.setSpecificBlue = rgb_SetSpecificBlue;
  lc.iRGBLed.in.setColour = rgb_SetColour;

  lc.iTimer.in.start = [](int sec){ unsigned suc = alarm(sec); assert(suc==0 && "A timer is already active..."); };
  lc.iTimer.in.cancel = [](){ alarm(0); };

  bool potmeter_active = false;

  lc.iPotMeter.in.start = [&](){ potmeter_active = true; };
  lc.iPotMeter.in.stop = [&](){ potmeter_active = false; };

  bool preset_btn = false;
  bool last_preset_btn = false;
  bool mix_btn = false;
  bool last_mix_btn = false;
  bool preset_btn_pressed = false;
  bool mix_btn_pressed = false;
  unsigned long last_dbnc_time = 0;

  int last_potmeter_value = 0;

  lc.iController.in.begin();

  while(true) { //forever
    if(timeout) {
      lc.iTimer.out.timeout();
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

    if((millis() - last_dbnc_time) > DEBOUNCE_TIME) {

      if(preset_btn_pressed != preset_btn) {
        preset_btn = preset_btn_pressed;

        if(preset_btn) {
          lc.iController.in.presetButtonPressed();
        }
      }

      if(mix_btn_pressed != mix_btn) {
        mix_btn = mix_btn_pressed;

        if(mix_btn) {
          lc.iController.in.mixButtonPressed();
        }
      }
    }

    if(potmeter_active) {
      
    }

    last_preset_btn = preset_btn_pressed;
    last_mix_btn = mix_btn_pressed;
  }
}
