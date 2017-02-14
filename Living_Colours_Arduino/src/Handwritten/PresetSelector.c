#include "PresetSelector.h"
#include "rgbLed.h"

typedef struct {
  int red;
  int green;
  int blue;
} Preset;

Preset presets[] = {  { .red = 255, .green = 255, .blue = 0 },
                      { .red = 0, .green = 255, .blue = 255 },
                      { .red = 255, .green = 0, .blue = 255 } };

void selectPreset(IPresetSelector* self, uint8_t p) {
  Preset current = presets[p];
  setColor(current.red, current.green, current.blue);
}
void stopPreset(IPresetSelector* self) {
  setColor(0, 0, 0);
}
