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
#ifndef COLOURSUPPLIER_HH
#define COLOURSUPPLIER_HH

#include "skel_ColourSupplier.hh"

#include <vector>

struct ColourSupplier : public skel::ColourSupplier
{
  std::vector<Colour> presets;
  int currentPreset;

  ColourSupplier(const dzn::locator&);
  private:
  void iColourSupplier_toggle();
  void iColourSupplier_getColour(Colour& colour);
};

#endif // COLOURSUPPLIER_HH
