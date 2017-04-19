#include <iostream>
#include <string>
#include <unistd.h>
#include <signal.h>

#include <dzn/runtime.hh>
#include <dzn/locator.hh>
#include "AlarmSystem.hh"

const std::string password = "1738";

std::function<void()> stop;
std::function<void()> timeout;

void sigint_handler(int signal) {
  stop();
  exit(signal);
}

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
  stop =  as.iController.in.stop;
  as.iTimer.in.start = [](int sec) { alarm(sec); };
  as.iTimer.in.cancel = []{ alarm(0); };
  timeout = as.iTimer.out.timeout;

  signal(SIGINT, sigint_handler);
  signal(SIGALRM, sigalrm_handler);

  as.iPasswordManager.in.VerifyPassword = [](std::string pw){ return pw.compare(password) == 0; };

  as.check_bindings();
  as.iController.in.start();

  std::string input;
  while(std::cin >> input) {
    as.iPasswordManager.out.PasswordEntered(input);
  }
}
