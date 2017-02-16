#ifndef TIMER_H
#define TIMER_H

#include "Arduino.h"
#include "../Generated/ITimer.h"

volatile bool timeout;

void tm_StartTimer(ITimer* self, long ms);
void tm_ClearTimer(ITimer* self);

#endif //TIMER_H
