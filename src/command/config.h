#ifndef _COMMAND_CONFIG_H
#define _COMMAND_CONFIG_H

#include <unordered_map>
#include <aria2/aria2.h>
#include "../command.h"

namespace command {
  class Config : public Command {
  public:
    Config(aria2::KeyVals&& options);
    virtual void execute(aria2::Session* session, std::unordered_map<aria2::A2Gid, std::string>* downloads);

  private:
    aria2::KeyVals options;
  };
}

#endif