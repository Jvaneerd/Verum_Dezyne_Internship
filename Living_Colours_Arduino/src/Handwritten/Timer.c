#include "Timer.h"

volatile bool timeout = false;

unsigned long determine_timer_compare_value(long period_in_ms) {
  long prescaler = 1024;
  long clock_speed = 16000000;

  return clock_speed / (prescaler * (1000/period_in_ms));
}

void startTimer(ITimer* self, long ms) {
  cli(); // stop interrupts

  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  OCR1A = determine_timer_compare_value(ms);

  TCCR1B |= _BV(WGM12) | _BV(CS12) | _BV(CS10);
  TIMSK1 |= _BV(OCIE1A);

  sei();
}

void clearTimer(ITimer* self) {
  cli();
  TIMSK1 &= ~_BV(OCIE1A);
  sei();
  timeout = false;
}

ISR(TIMER1_COMPA_vect){
  timeout = true;
}
