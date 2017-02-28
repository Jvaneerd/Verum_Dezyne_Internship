// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#include "../Generated/PresetList.hh"

#include <dzn/locator.hh>
#include <dzn/runtime.hh>

#include <vector>

std::vector<Colour> presets;
int currentPreset;

PresetList::PresetList(const dzn::locator& dezyne_locator)
: dzn_meta{"","PresetList",0,{},{[this]{iPresetList.check_bindings();}}}
, dzn_rt(dezyne_locator.get<dzn::runtime>())
, dzn_locator(dezyne_locator)
, iPresetList{{{"iPresetList",this,&dzn_meta},{"",0,0}}}
{
  dzn_rt.performs_flush(this) = true;
  iPresetList.in.toggle = [&] () { return dzn::call_in(this, [&]{return iPresetList_toggle();}, this->iPresetList.meta, "toggle"); };
  iPresetList.in.getPreset = [&] (Colour& colour) { return dzn::call_in(this, [&]{return iPresetList_getPreset(colour);}, this->iPresetList.meta, "getPreset"); };

  presets.push_back( { .red = 100, .green = 100, .blue = 0 } ); //Yellow
  presets.push_back( { .red = 0, .green = 100, .blue = 100 } ); //Cyan
  presets.push_back( { .red = 100, .green = 0, .blue = 100 } ); //Magenta
  currentPreset = 0;
}

void PresetList::iPresetList_toggle()
{
  currentPreset++;
  if(currentPreset == presets.size()) currentPreset = 0;
}

void PresetList::iPresetList_getPreset(Colour& colour)
{
  colour = presets.at(currentPreset);
}


void PresetList::check_bindings() const
{
  dzn::check_bindings(&dzn_meta);
}
void PresetList::dump_tree(std::ostream& os) const
{
  dzn::dump_tree(os, &dzn_meta);
}
