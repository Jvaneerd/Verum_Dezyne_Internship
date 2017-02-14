#ifndef HWMIXER_H
#define HWMIXER_H

#include "../Generated/IMixer.h"
#include "../Generated/IHWMixer.h"

int colours[3];
uint8_t currentColour;
bool hwMixer_Started;

void hwMixer_Start(IHWMixer *self);
void hwMixer_Stop(IHWMixer *self);

void hwMixer_mixRed(IHWMixer *self);
void hwMixer_mixGreen(IHWMixer *self);
void hwMixer_mixBlue(IHWMixer *self);

bool hwMixer_checkChanges();

#endif //HWMIXER_H
