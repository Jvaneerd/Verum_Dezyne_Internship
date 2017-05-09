#include <iostream>
#include <string>
#include <unistd.h>
#include <signal.h>

#include <dzn/runtime.hh>
#include <dzn/locator.hh>
#include "AlarmSystem.hh"

std::function<void()> timeout;

void sigalrm_handler(int signal) {
  timeout();
}

int
main(int argc, char* argv[])
{
  dzn::locator loc;
  dzn::runtime rt;

  loc.set(rt);

  AlarmSystem as(loc);
  as.dzn_meta.name = "AlarmSystem";

  as.iTimer.in.start = [] (int ms) { alarm(ms/1000); }; //ITimer contract is in milliseconds, but alarm is in seconds
  as.iTimer.in.cancel = [] () { alarm(0); };
  timeout = as.iTimer.out.timeout;
  signal(SIGALRM, sigalrm_handler);

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
