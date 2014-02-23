#include <aria2/aria2.h>

#include "../event.h"
#include "stop.h"
#include "../download.h"

using namespace event;
using namespace std;
using namespace v8;
using namespace node;

Stop::Stop(string uid) : uid(uid) {
}

void Stop::on(unordered_map<string, Persistent<Object>>* downloads) {
  Download* download = ObjectWrap::Unwrap<Download>(downloads->at(uid));
  download->processOnStop();
}