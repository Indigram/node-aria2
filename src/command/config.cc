#include <aria2/aria2.h>
#include "../command.h"
#include "config.h"

using namespace aria2;
using namespace command;

Config::Config(KeyVals&& options) : options(options) {
}

void Config::execute(aria2::Session* session, std::unordered_map<aria2::A2Gid, std::string>* downloads) {
  assert(changeGlobalOption(session, options) == 0);
}