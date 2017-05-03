#include <iostream>
#include <string>
#include <unistd.h>
#include <signal.h>

#include <dzn/runtime.hh>
#include <dzn/locator.hh>
#include <dzn/pump.hh>
#include "AlarmSystem.hh"

std::string password = "wachtwoord";

int
main(int argc, char* argv[])
{
  dzn::locator loc;
  dzn::pump pump;
  dzn::runtime rt;

  AlarmSystem as(loc.set(rt).set(pump));
  as.dzn_meta.name = "AlarmSystem";

  as.iPWManager.in.verifyPassword = [](std::string pw) { return pw.compare(password) == 0; };

  as.check_bindings();

  std::string input;
  while(std::cin >> input) {
    if(input.compare("s") == 0) {
      as.iController.in.sensorTriggered();
    }
    else {
      as.iController.in.passwordEntered(input);
    }
  }
}
