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
#include "ColourSupplier.hh"

ColourSupplier::ColourSupplier(const dzn::locator& dezyne_locator) : skel::ColourSupplier(dezyne_locator)
{
  presets.push_back( { .red = 100, .green = 100, .blue = 0 } ); //Yellow
  presets.push_back( { .red = 0, .green = 100, .blue = 100 } ); //Cyan
  presets.push_back( { .red = 100, .green = 0, .blue = 100 } ); //Magenta
  currentPreset = 0;
}

void ColourSupplier::iColourSupplier_toggle()
{
  currentPreset = (currentPreset + 1) % presets.size();
  // currentPreset++;
  // if(currentPreset == presets.size()) currentPreset = 0;
}

void ColourSupplier::iColourSupplier_getColour(Colour& colour)
{
  colour = presets[currentPreset];
}
