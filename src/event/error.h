#ifndef _EVENT_ERROR_H
#define _EVENT_ERROR_H

#include <aria2/aria2.h>
#include "../event.h"

namespace event {
  class Error : public Event {
  public:
    Error(std::string uid, int code);
    virtual void on(std::unordered_map<std::string, v8::Persistent<v8::Object>>* downloads);
  private:
    std::string uid;
    int code;
  };
}

#endif