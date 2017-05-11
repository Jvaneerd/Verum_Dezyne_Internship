#include <iostream>
#include <string>
#include <unistd.h>
#include <signal.h>

#include <dzn/runtime.hh>
#include <dzn/locator.hh>
#include "AlarmSystem.hh"

int
main(int argc, char* argv[])
{
  dzn::locator loc;
  dzn::runtime rt;

  loc.set(rt);

  AlarmSystem as(loc);

  as.check_bindings();

  std::string input;
  while(std::cin >> input) {
    if(input.compare("s") == 0) {
      as.iController.in.sensorTriggered();
    }
    else if(input.compare("v") == 0) {
      as.iController.in.validPincode();
    }
  }
}
