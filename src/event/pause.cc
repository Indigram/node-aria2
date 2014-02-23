#include <aria2/aria2.h>

#include "../event.h"
#include "pause.h"
#include "../download.h"

using namespace event;
using namespace std;
using namespace v8;
using namespace node;

Pause::Pause(string uid) : uid(uid) {
}

void Pause::on(unordered_map<string, Persistent<Object>>* downloads) {
  Download* download = ObjectWrap::Unwrap<Download>(downloads->at(uid));
  download->processOnPause();
}