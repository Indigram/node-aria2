#ifndef _COMMAND_SHUTDOWN_H
#define _COMMAND_SHUTDOWN_H

#include <unordered_map>
#include <aria2/aria2.h>
#include "../command.h"

namespace command {
  class Shutdown : public Command {
  public:
    Shutdown();
    virtual void execute(aria2::Session* session, std::unordered_map<aria2::A2Gid, std::string>* downloads);
  };
}

#endif