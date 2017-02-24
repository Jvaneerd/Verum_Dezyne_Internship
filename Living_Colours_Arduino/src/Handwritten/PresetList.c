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
#include "../Generated/PresetList.h"

#include <dzn/locator.h>
#include <dzn/runtime.h>
#include <string.h>

Colour presets[] = {  { .red = 255, .green = 255, .blue = 0 }, //Yellow
                      { .red = 0, .green = 255, .blue = 255 }, //Cyan
                      { .red = 255, .green = 0, .blue = 255 }, //Magenta
                   };

#define NR_OF_PRESETS sizeof(presets)/sizeof(Colour)

int currentPreset = 0;

static void iPresetList_toggle(PresetList* self) {
	(void)self;
  {
	  currentPreset++;
	  if(currentPreset == NR_OF_PRESETS) {
	    currentPreset = 0;
	  }
	}
}

static void iPresetList_getPreset(PresetList* self,Colour* colour) {
	(void)self;
	{
    *colour = presets[currentPreset];
	}
}

static void call_in_iPresetList_toggle(IPresetList* port) {
	RUNTIME_TRACE_in(&port->meta, "toggle");
	PresetList* self_ = port->meta.provides.address;
	runtime_start(&self_->dzn_info);
	iPresetList_toggle(self_);
	runtime_finish(&self_->dzn_info);
	RUNTIME_TRACE_out(&port->meta, "return");
}
static void call_in_iPresetList_getPreset(IPresetList* port,Colour* colour) {
	RUNTIME_TRACE_in(&port->meta, "getPreset");
	PresetList* self_ = port->meta.provides.address;
	runtime_start(&self_->dzn_info);
	iPresetList_getPreset(self_,colour);
	runtime_finish(&self_->dzn_info);
	RUNTIME_TRACE_out(&port->meta, "return");
}

void PresetList_init (PresetList* self, locator* dezyne_locator) {
	runtime_info_init(&self->dzn_info, dezyne_locator);
	self->dzn_info.performs_flush = false;

	self->iPresetList->in.toggle = iPresetList_toggle;
	self->iPresetList->in.getPreset = iPresetList_getPreset;
}
