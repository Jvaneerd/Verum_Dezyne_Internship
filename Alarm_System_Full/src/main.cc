#include <iostream>
#include <string>
#include <unistd.h>
#include <signal.h>

#include <dzn/runtime.hh>
#include <dzn/locator.hh>
#include <dzn/pump.hh>
#include "AlarmSystem.hh"

std::string password = "wachtwoord";

std::function<void()> timeout;

void sigalrm_handler(int signal) {
  timeout();
}

int
main(int argc, char* argv[])
{
  dzn::locator loc;
  dzn::pump pump;
  dzn::runtime rt;

  AlarmSystem as(loc.set(rt).set(pump));
  as.dzn_meta.name = "AlarmSystem";

  as.iPWManager.in.verifyPassword = [&](std::string pw){ return pw.compare(password) == 0; };

  bool sensorPolling = false;
  as.iSensor.in.turnOn = [&](){ sensorPolling = true; };
  as.iSensor.in.turnOff = [&](){ sensorPolling = false; };

  as.iTimer.in.start = [](int milliseconds){ alarm(milliseconds/1000); };  // ITimer is in milliseconds, but alarm is in seconds
  as.iTimer.in.cancel = []{ alarm(0); };
  timeout = as.iTimer.out.timeout;
  signal(SIGALRM, sigalrm_handler);

  as.check_bindings();

  std::string input;
  while(std::cin >> input) {
    if(input.compare("s") == 0 ) {
      if(sensorPolling) as.iSensor.out.triggered();
    }
    else {
      as.iController.in.passwordEntered(input);
    }
  }
}
