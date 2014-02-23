#include <aria2/aria2.h>

#include "../event.h"
#include "progress.h"
#include "../download.h"

using namespace event;
using namespace std;
using namespace v8;
using namespace node;

Progress::Progress(string uid, int total, int completed, int speed) :
uid(uid),
total(total),
completed(completed),
speed(speed) {
}

void Progress::on(unordered_map<string, Persistent<Object>>* downloads) {
  Download* download = ObjectWrap::Unwrap<Download>(downloads->at(uid));
  download->processOnProgress(total, completed, speed);
}