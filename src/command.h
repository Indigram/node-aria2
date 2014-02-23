#ifndef _COMMAND_H
#define _COMMAND_H

#include <unordered_map>
#include <aria2/aria2.h>

class Command {
public:
  virtual ~Command();
  virtual void execute(aria2::Session* session, std::unordered_map<aria2::A2Gid, std::string>* downloads) = 0;
};

#endif