#ifndef _EVENT_STOP_H
#define _EVENT_STOP_H

#include <aria2/aria2.h>
#include "../event.h"

namespace event {
  class Stop : public Event {
  public:
    Stop(std::string uid);
    virtual void on(std::unordered_map<std::string, v8::Persistent<v8::Object>>* downloads);
  private:
    std::string uid;
  };
}

#endif