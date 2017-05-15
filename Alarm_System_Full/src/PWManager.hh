#ifndef PWMANAGER_HH
#define PWMANAGER_HH

#include "skel_PWManager.hh"

class PWManager : public skel::PWManager {
  const std::string password = "Dezyne";

  bool iPWManager_verifyPassword(std::string pw);
public:
  PWManager(const dzn::locator& loc);
};

#endif //PWMANAGER_HH
