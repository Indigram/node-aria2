#include <aria2/aria2.h>

#include "../event.h"
#include "error.h"
#include "../download.h"

using namespace event;
using namespace std;
using namespace v8;
using namespace node;

Error::Error(string uid, int code) : uid(uid), code(code) {
}

void Error::on(unordered_map<string, Persistent<Object>>* downloads) {
  Download* download = ObjectWrap::Unwrap<Download>(downloads->at(uid));
  download->processOnError(code);
}