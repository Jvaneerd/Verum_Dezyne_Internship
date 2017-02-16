#include "PresetSelector.h"
#include "rgbLed.h"

Colour presets[] = {  { .red = 255, .green = 255, .blue = 0 }, //Yellow
                      { .red = 0, .green = 255, .blue = 255 }, //Cyan
                      { .red = 255, .green = 0, .blue = 255 }, //Magenta
                   };

#define NR_OF_PRESETS sizeof(presets)/sizeof(Colour)

int currentPreset = 0;

void ps_Toggle(IPresetSelector* self) {
  currentPreset++;
  if(currentPreset == NR_OF_PRESETS) {
    currentPreset = 0;
  }
}

void ps_GetPreset(IPresetSelector* self, Colour* colour) {
  *colour = presets[currentPreset];
}
