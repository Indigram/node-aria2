#ifndef _EVENT_START_H
#define _EVENT_START_H

#include <aria2/aria2.h>
#include "../event.h"

namespace event {
  class Start : public Event {
  public:
    Start(std::string uid);
    virtual void on(std::unordered_map<std::string, v8::Persistent<v8::Object>>* downloads);
  private:
    std::string uid;
  };
}

#endif