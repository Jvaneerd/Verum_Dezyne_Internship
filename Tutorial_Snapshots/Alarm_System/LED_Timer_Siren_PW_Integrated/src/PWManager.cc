#include "PWManager.hh"

PWManager::PWManager(const dzn::locator& loc) : skel::PWManager(loc) {
  //no op
}

bool PWManager::iPWManager_verifyPassword(std::string pw) {
  return pw.compare(this->password) == 0;
}
