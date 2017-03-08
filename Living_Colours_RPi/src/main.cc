#include <dzn/runtime.hh>
#include <dzn/locator.hh>

#include <unistd.h>
#include <signal.h>
#include <wiringPi.h>

#include "LivingColoursSystem.hh"
#include "hardware.hh"

volatile bool timeoutOccurred = false;

int main(void) {
	signal(SIGALRM, [](int signal){ timeoutOccurred = true; });
	dzn::locator loc;
  dzn::runtime rt;
	dzn::illegal_handler illegal_handler;

  LivingColoursSystem lc(loc.set(rt).set(illegal_handler));
	RgbLed rgbLed;

	lc.iRGBLed.in.setRed = [&](int value){rgbLed.SetRed(value);};
	lc.iRGBLed.in.setGreen = [&](int value){rgbLed.SetGreen(value);};
	lc.iRGBLed.in.setBlue = [&](int value){rgbLed.SetBlue(value);};
	lc.iRGBLed.in.setColour = [&](Colour colour){rgbLed.SetColour(colour);};
	lc.iRGBLed.in.dim = [&]{rgbLed.Dim();};

  lc.iTimer.in.start = [](int sec){ alarm(sec); };
  lc.iTimer.in.cancel = [](){ alarm(0); };

  bool pm_active = false;
  int last_pm_value = -1;
  PotMeter potMeter;

  lc.iPotMeter.in.start = [&](){ pm_active = true; };
  lc.iPotMeter.in.stop = [&](){ pm_active = false; };

  Buttons buttons;

  int last_potmeter_value = 0;

	lc.check_bindings();
	lc.iController.in.begin();

	while(true) { //infinite event loop
		if(timeoutOccurred == true) {
			lc.iTimer.out.timeout();
			timeoutOccurred = false;
		}

    bool btn_event = buttons.pollButtons();

    if(btn_event && buttons.preset_btn) {
      lc.iController.in.presetButtonPressed();
    }
    else if(btn_event && buttons.mix_btn) {
      lc.iController.in.mixButtonPressed();
    }

    if(pm_active) {
      int current_potmeter_value = potMeter.read();
      if(current_potmeter_value != last_potmeter_value) {
        int mappedPotVal = potMeter.map(current_potmeter_value, 0, 100);
        lc.iPotMeter.out.valueChanged(mappedPotVal);
        last_potmeter_value = current_potmeter_value;
      }
    }
	}
}
