#ifndef TIMER_H
#define TIMER_H

#include "Arduino.h"
#include "../Generated/ITimer.h"

volatile bool timeout;

void startTimer(ITimer* self, long ms);
void clearTimer(ITimer* self);

#endif //TIMER_H
