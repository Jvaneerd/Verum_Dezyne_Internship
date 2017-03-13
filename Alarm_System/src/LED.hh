#ifndef LED_HH
#define LED_HH

#include "skel_LED.hh"

class LED : public skel::LED {
  const int PIN_RED = 27;
  const int PIN_GREEN = 28;
  const int PIN_BLUE =  29;

public:
  LED(const dzn::locator& loc);
  void iLED_setGreen();
  void iLED_setYellow();
  void iLED_setRed();
  void iLED_turnOff();
};

#endif //LED_HH
