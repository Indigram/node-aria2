#ifndef _EVENT_PROGRESS_H
#define _EVENT_PROGRESS_H

#include <aria2/aria2.h>
#include "../event.h"

namespace event {
  class Progress : public Event {
  public:
    Progress(std::string uid, int total, int completed, int speed);
    virtual void on(std::unordered_map<std::string, v8::Persistent<v8::Object>>* downloads);
  private:
    std::string uid;
    int total;
    int completed;
    int speed;
  };
}

#endif