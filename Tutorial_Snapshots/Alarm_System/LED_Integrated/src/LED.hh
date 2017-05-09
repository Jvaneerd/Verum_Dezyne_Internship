#ifndef LED_HH
#define LED_HH

#include "skel_LED.hh"

class LED : public skel::LED {
  const int PIN_RED = 27;
  const int PIN_GREEN = 28;
  const int PIN_BLUE =  29;

  void iLed_setGreen();
  void iLed_setYellow();
  void iLed_setRed();
  void iLed_turnOff();
  
public:
  LED(const dzn::locator& loc);
};

#endif //LED_HH
