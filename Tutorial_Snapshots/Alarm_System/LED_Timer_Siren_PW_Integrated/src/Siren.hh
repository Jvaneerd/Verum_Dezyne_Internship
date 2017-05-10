#ifndef SIREN_HH
#define SIREN_HH

#include "skel_Siren.hh"

class Siren : public skel::Siren {
public:
  Siren(const dzn::locator& loc);
  void iSiren_turnOn();
  void iSiren_turnOff();
};

#endif // SIREN_HH
