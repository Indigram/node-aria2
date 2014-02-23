#ifndef _COMMAND_ADD_H
#define _COMMAND_ADD_H

#include <unordered_map>
#include <aria2/aria2.h>
#include "../command.h"

namespace command {
  class Add : public Command {
  public:
    Add(std::string uri, std::string uid);
    virtual void execute(aria2::Session* session, std::unordered_map<aria2::A2Gid, std::string>* downloads);

  private:
    std::string uri;
    std::string uid;
  };
}


#endif