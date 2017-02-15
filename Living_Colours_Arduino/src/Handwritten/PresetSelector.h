#ifndef PRESETSELECTOR_H
#define PRESETSELECTOR_H

#include "../Generated/IPresetSelector.h"

void ps_Toggle(IPresetSelector* self);
void ps_GetPreset(IPresetSelector* self, Colour* colour);

#endif //PRESETSELECTOR_H
