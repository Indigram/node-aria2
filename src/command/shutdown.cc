#include <aria2/aria2.h>
#include "../command.h"
#include "shutdown.h"

using namespace aria2;
using namespace command;

Shutdown::Shutdown() {
}

void Shutdown::execute(aria2::Session* session, std::unordered_map<aria2::A2Gid, std::string>* downloads) {
  shutdown(session);
}