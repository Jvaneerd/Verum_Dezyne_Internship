#include <iostream>

#include "Siren.hh"

Siren::Siren(const dzn::locator& loc) : skel::Siren(loc) {
  //no op
}

void Siren::iSiren_turnOn() {
  std::cout << "SIREN >>ACTIVATED<<" << std::endl;
}

void Siren::iSiren_turnOff() {
  std::cout << "SIREN >>DEACTIVATED<<" << std::endl;
}
