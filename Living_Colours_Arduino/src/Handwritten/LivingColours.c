#include "Arduino.h"
#include <assert.h>
#include <dzn/runtime.h>
#include <dzn/locator.h>

#include "../Generated/LivingColours.h"

#define PIN_RED 11
#define PIN_GREEN 6
#define PIN_BLUE 5

volatile bool timeout = false;

void startTimer(ITimer* self, long ms) {
  cli(); // stop interrupts

  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  OCR1A = 15624;

  TCCR1B |= _BV(WGM12) | _BV(CS12) | _BV(CS10);
  TIMSK1 |= _BV(OCIE1A);

  sei();
}

void clearTimer(ITimer* self) {
  cli();
  TIMSK1 &= ~_BV(OCIE1A);
  sei();
}

ISR(TIMER1_COMPA_vect){
  timeout = true;
}

void arduinoSetup() {
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
  pinMode(PIN_DEBUG, OUTPUT);
  pinMode(PIN_ILLEGAL, OUTPUT);
  digitalWrite(PIN_DEBUG, LOW);
  digitalWrite(PIN_ILLEGAL, LOW);
}

void setColor(int red, int green, int blue) {
  analogWrite(PIN_RED, red);
  analogWrite(PIN_GREEN, green);
  analogWrite(PIN_BLUE, blue);
}

void rgb_setRed(IRGBLed* self) {
  setColor(255, 0, 0);
}

void rgb_setGreen(IRGBLed* self) {
  setColor(0, 255, 0);
}

void rgb_setBlue(IRGBLed* self) {
  setColor(0, 0, 255);
}

void timer_timeout(ITimer* self) {
  timeout = false; //handled in Dezyne
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

  while(1) {
    if(timeout) {
      lc.iTimer->out.timeout(lc.iTimer);
      timeout = false;
    }
    //forever
  }

  return 0;
}
