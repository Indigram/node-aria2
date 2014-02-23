#include <aria2/aria2.h>

#include "../event.h"
#include "start.h"
#include "../download.h"

using namespace event;
using namespace std;
using namespace v8;
using namespace node;

Start::Start(string uid) : uid(uid) {
}

void Start::on(unordered_map<string, Persistent<Object>>* downloads) {
  Download* download = ObjectWrap::Unwrap<Download>(downloads->at(uid));
  download->processOnStart();
}