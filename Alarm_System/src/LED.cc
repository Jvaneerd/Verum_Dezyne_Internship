#include <wiringPi.h>
#include "LED.hh"

LED::LED(const dzn::locator& loc) : skel::LED(loc) {
  wiringPiSetup();
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
}

void LED::iLED_setGreen() {
  digitalWrite(PIN_RED, LOW);
  digitalWrite(PIN_GREEN, HIGH);
  digitalWrite(PIN_BLUE, LOW);
}

void LED::iLED_setYellow() {
  digitalWrite(PIN_RED, HIGH);
  digitalWrite(PIN_GREEN, HIGH);
  digitalWrite(PIN_BLUE, LOW);
}

void LED::iLED_setRed() {
  digitalWrite(PIN_RED, HIGH);
  digitalWrite(PIN_GREEN, LOW);
  digitalWrite(PIN_BLUE, LOW);
}

void LED::iLED_turnOff() {
  digitalWrite(PIN_RED, LOW);
  digitalWrite(PIN_GREEN, LOW);
  digitalWrite(PIN_BLUE, LOW);
}
