#ifndef _COMMAND_START_H
#define _COMMAND_START_H

#include <aria2/aria2.h>
#include "../command.h"

namespace command {
  class Start : public Command {
  public:
    Start(std::string uid);
    virtual void execute(aria2::Session* session, std::unordered_map<aria2::A2Gid, std::string>* downloads);
  private:
    std::string uid;
  };
}


#endif