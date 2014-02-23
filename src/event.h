#ifndef _EVENT_H
#define _EVENT_H

#include <unordered_map>
#include <node.h>

class Event {
public:
  virtual ~Event();
  virtual void on(std::unordered_map<std::string, v8::Persistent<v8::Object>>* downloads) = 0;
};

#endif