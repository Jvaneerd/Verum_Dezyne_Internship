#ifndef POTMETER_H
#define POTMETER_H

#include "../Generated/IPotMeter.h"

int currentValue;
bool pm_Started;

void pm_Start(IPotMeter *self);
void pm_Stop(IPotMeter *self);

bool pm_CheckChanges();

#endif //potMeter_H
