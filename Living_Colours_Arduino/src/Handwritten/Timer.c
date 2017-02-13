#include "Timer.h"

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
