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
  Persistent<Object> downloadObject = downloads->at(uid);
  Download* download = ObjectWrap::Unwrap<Download>(downloadObject);
  download->processOnError(code);

  downloadObject.MakeWeak(NULL, NULL);
  downloads->erase(uid);
}