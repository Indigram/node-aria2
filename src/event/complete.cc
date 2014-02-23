#include <aria2/aria2.h>

#include "../event.h"
#include "complete.h"
#include "../download.h"

using namespace event;
using namespace std;
using namespace v8;
using namespace node;

Complete::Complete(string uid, string path) : uid(uid), path(path) {
}

void Complete::on(unordered_map<string, Persistent<Object>>* downloads) {
  Download* download = ObjectWrap::Unwrap<Download>(downloads->at(uid));
  download->processOnComplete(path);
}