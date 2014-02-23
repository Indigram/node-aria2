#ifndef _EVENT_COMPLETE_H
#define _EVENT_COMPLETE_H

#include <aria2/aria2.h>
#include "../event.h"

namespace event {
  class Complete : public Event {
  public:
    Complete(std::string uid, std::string path);
    virtual void on(std::unordered_map<std::string, v8::Persistent<v8::Object>>* downloads);
  private:
    std::string uid;
    std::string path;
  };
}

#endif