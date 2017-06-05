/*
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "../Generated/ColourSupplier.h"

#include <dzn/locator.h>
#include <dzn/runtime.h>
#include <string.h>

Colour presets[] = {  { .red = 255, .green = 255, .blue = 0 }, //Yellow
                      { .red = 0, .green = 255, .blue = 255 }, //Cyan
                      { .red = 255, .green = 0, .blue = 255 }, //Magenta
                   };

#define NR_OF_PRESETS sizeof(presets)/sizeof(Colour)

int currentPreset = 0;

static void iColourSupplier_toggle(IColourSupplier* self) {
	(void)self;
  {
	  currentPreset++;
	  if(currentPreset == NR_OF_PRESETS) {
	    currentPreset = 0;
	  }
	}
}

static void iColourSupplier_getColour(IColourSupplier* self,Colour* colour) {
	(void)self;
	{
    *colour = presets[currentPreset];
	}
}

void ColourSupplier_init (ColourSupplier* self, locator* dezyne_locator) {
	runtime_info_init(&self->dzn_info, dezyne_locator);
	self->dzn_info.performs_flush = false;

	self->iColourSupplier->in.toggle = iColourSupplier_toggle;
	self->iColourSupplier->in.getColour = iColourSupplier_getColour;
}
