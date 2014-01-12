#include <assert.h>
#include <aria2/aria2.h>
#include <node.h>
#include "aria2.h"
#include "download.h"

using namespace std;
using namespace aria2;
using namespace v8;

Persistent<Function> Aria2::constructor;

void Aria2::Init() {
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::New("Aria2"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  tpl->PrototypeTemplate()->Set(String::New("config"),
      FunctionTemplate::New(Config)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::New("download"),
      FunctionTemplate::New(Download)->GetFunction());

  constructor = Persistent<Function>::New(tpl->GetFunction());
}

Handle<Value> Aria2::NewInstance() {
  HandleScope scope;
  Local<Object> instance = constructor->NewInstance(0, NULL);
  return scope.Close(instance);
}

Aria2::Aria2() {
}

Aria2::~Aria2() {
}

void Aria2::init() {
  assert(libraryInit() == 0);
  SessionConfig config;
  config.downloadEventCallback = EventCallback;
  config.useSignalHandler = false;
  config.keepRunning = true;
  config.userData = &this->downloads;
  this->session = aria2::sessionNew(aria2::KeyVals(), config);

}

void Aria2::destroy() {
  sessionFinal(this->session);
  assert(libraryDeinit() == 0);
}

void Aria2::start() {

  uv_timer_init(uv_default_loop(), &this->runTimer);
  this->runTimer.data = this->session;
  uv_timer_start(&this->runTimer, RunWork, 0, 500);
  uv_run(uv_default_loop(), UV_RUN_NOWAIT);
}

void Aria2::stop() {
  shutdown(this->session);
}

Handle<Value> Aria2::New(const Arguments& args) {
  HandleScope scope;
  if (args.IsConstructCall()) {
    Aria2* aria2 = new Aria2();
    aria2->Wrap(args.This());
    return args.This();
  } else {
    return scope.Close(constructor->NewInstance(0, NULL));
  }
}

Handle<Value> Aria2::Config(const Arguments& args) {
  HandleScope scope;

  if (args.Length() < 1) {
    return scope.Close(Undefined());
  }

  KeyVals options;
  if (args[0]->IsObject()) {
    Handle<Object> object = Handle<Object>::Cast(args[0]);
    Handle<Array> propertyNames = object->GetPropertyNames();
    for (uint32_t i = 0; i < propertyNames->Length(); i++) {
      Handle<Value> property = propertyNames->Get(i)->ToString();
      Handle<Value> value = object->Get(propertyNames->Get(i)->ToString());
      options.push_back(KeyVals::value_type(
          *(String::Utf8Value(property->ToString())),
          *(String::Utf8Value(value->ToString()))
      ));
    }
  }

  Aria2* aria = ObjectWrap::Unwrap<Aria2>(args.This());
  assert(changeGlobalOption(aria->session, options) == 0);

  return scope.Close(Undefined());
}

Handle<Value> Aria2::Download(const Arguments& args) {
  HandleScope scope;

  Aria2* aria = ObjectWrap::Unwrap<Aria2>(args.This());
  Handle<Value> session = External::New(aria->session);
  Handle<Value> download = Download::NewInstance(args, session);

  aria->downloads.insert(make_pair(
      ObjectWrap::Unwrap<class::Download>(download->ToObject())->getGid(),
      Persistent<Object>::New(download->ToObject())
  ));

  return scope.Close(download);
}

int Aria2::EventCallback(Session* session, DownloadEvent event, A2Gid gid, void* arg) {
  unordered_map<A2Gid, Persistent<Object>>* downloads = static_cast<unordered_map<A2Gid, Persistent<Object>>*>(arg);
  Persistent<Object> downloadObject = downloads->at(gid);
  class::Download* download = ObjectWrap::Unwrap<class::Download>(downloadObject);
  download->onEvent(event);

  if (event == EVENT_ON_DOWNLOAD_STOP ||
      event == EVENT_ON_DOWNLOAD_COMPLETE ||
      event == EVENT_ON_DOWNLOAD_ERROR) {
    downloadObject.MakeWeak(NULL, NULL);
    downloads->erase(gid);
  }
  return 0;
}

void Aria2::RunWork(uv_timer_t* runTimer, int status) {
  Session* session = (Session*)runTimer->data;
  if (run(session, RUN_ONCE) != 1) {
    uv_timer_stop(runTimer);
  }
}