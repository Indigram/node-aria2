#include <assert.h>
#include <string>
#include <aria2/aria2.h>
#include <node.h>
#include "download.h"

using namespace std;
using namespace aria2;
using namespace v8;
using namespace node;

Persistent<Function> Download::constructor;

void Download::Init() {
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::New("Download"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  tpl->PrototypeTemplate()->Set(String::New("on"),
      FunctionTemplate::New(On)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::New("start"),
      FunctionTemplate::New(Start)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::New("pause"),
      FunctionTemplate::New(Pause)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::New("remove"),
      FunctionTemplate::New(Remove)->GetFunction());

  constructor = Persistent<Function>::New(tpl->GetFunction());
}

Handle<Value> Download::NewInstance(const Arguments& args, const Handle<Value> session) {
  HandleScope scope;

  const int argc = 2;
  Handle<Value> argv[argc] = {args[0], session};
  Local<Object> instance = constructor->NewInstance(argc, argv);

  return scope.Close(instance);
}

Download::Download(Session* session) :
session(session),
callbackStart(NULL),
callbackPause(NULL),
callbackStop(NULL),
callbackComplete(NULL),
callbackError(NULL),
callbackProgress(NULL) {
}

Download::~Download() {
  DisposeFunction(&this->callbackStart);
  DisposeFunction(&this->callbackPause);
  DisposeFunction(&this->callbackStop);
  DisposeFunction(&this->callbackComplete);
  DisposeFunction(&this->callbackError);
  DisposeFunction(&this->callbackProgress);
}

void Download::onEvent(DownloadEvent event) {
switch (event) {
    case EVENT_ON_DOWNLOAD_START:
      this->onStart();
      break;
    case EVENT_ON_DOWNLOAD_PAUSE:
      this->onPause();
      break;
    case EVENT_ON_DOWNLOAD_STOP:
      this->onStop();
      break;
    case EVENT_ON_DOWNLOAD_COMPLETE:
      this->onComplete();
      break;
    case EVENT_ON_DOWNLOAD_ERROR:
      this->onError();
      break;
    default:
      break;
  }
}

A2Gid Download::getGid() {
  return this->gid;
}

Handle<Value> Download::New(const Arguments& args) {
  HandleScope scope;
  if (args.IsConstructCall()) {
    if (args.Length()<2) {
      ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
      return scope.Close(Undefined());
    }

    if (!args[0]->IsString()) {
      ThrowException(Exception::TypeError(String::New("Wrong arguments")));
    }

    String::Utf8Value uri(args[0]->ToString());
    Session* session = static_cast<Session*>(External::Unwrap(args[1]));

    Download* download = new Download(session);
    download->addUri(*uri);
    download->Wrap(args.This());

    return args.This();
  } else {
    return scope.Close(constructor->NewInstance(0, NULL));
  }
}

Handle<Value> Download::On(const Arguments& args) {
  HandleScope scope;

  if (args.Length()<2) {
    ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
    return scope.Close(Undefined());
  }

  if (!args[0]->IsString() || !args[1]->IsFunction()) {
    if (!args[0]->IsString()) {
      ThrowException(Exception::TypeError(String::New("Wrong arguments")));
    }
  }

  Download* download = ObjectWrap::Unwrap<Download>(args.This());
  string event = string(*String::Utf8Value(args[0]->ToString()));

  if (event == "start")
    ReplaceFunction(&download->callbackStart, args[1]);
  else if (event == "pause")
    ReplaceFunction(&download->callbackPause, args[1]);
  else if (event == "stop")
    ReplaceFunction(&download->callbackStop, args[1]);
  else if (event == "complete")
    ReplaceFunction(&download->callbackComplete, args[1]);
  else if (event == "error")
    ReplaceFunction(&download->callbackError, args[1]);
  else if (event == "progress")
    ReplaceFunction(&download->callbackProgress, args[1]);

  return scope.Close(Undefined());
}

Handle<Value> Download::Start(const Arguments& args) {
  HandleScope scope;

  Download* download = ObjectWrap::Unwrap<Download>(args.This());
  download->start();

  return scope.Close(Undefined());
}

Handle<Value> Download::Pause(const Arguments& args) {
  HandleScope scope;

  Download* download = ObjectWrap::Unwrap<Download>(args.This());
  download->pause();

  return scope.Close(Undefined());
}

Handle<Value> Download::Remove(const Arguments& args) {
  HandleScope scope;

  Download* download = ObjectWrap::Unwrap<Download>(args.This());
  download->remove();

  return scope.Close(Undefined());
}

void Download::ProgressWork(uv_timer_t* progressTimer, int status) {
  Download* download = static_cast<Download*>(progressTimer->data);
  download->onProgress();
}

void Download::ReplaceFunction(Persistent<Function>** destination, Handle<Value> source) {
  if (*destination != NULL) {
    (*destination)->Dispose();
    delete *destination;
  }
  *destination = new Persistent<Function>();
  **destination = Persistent<Function>::New(Handle<Function>::Cast(source));
}

void Download::DisposeFunction(Persistent<Function>** function) {
  if (*function != NULL) {
    (*function)->Dispose();
    delete *function;
  }
}

void Download::addUri(string uri) {
  A2Gid gid;
  std::vector<std::string> uris;
  uris.push_back(uri);
  assert(aria2::addUri(this->session, &gid, uris, KeyVals()) == 0);
  assert(pauseDownload(this->session, gid, false) == 0);
  this->gid = gid;
}

void Download::start() {
  assert(unpauseDownload(this->session, this->gid) == 0);
  uv_timer_init(uv_default_loop(), &this->progressTimer);
  this->progressTimer.data = this;
}

void Download::pause() {
  assert(pauseDownload(this->session, this->gid) == 0);
}

void Download::remove() {
  assert(removeDownload(this->session, this->gid) == 0);
}

void Download::onStart() {
  uv_timer_start(&this->progressTimer, ProgressWork, 0, 1000);

  if (this->callbackStart)
    (*(this->callbackStart))->Call(Context::GetCurrent()->Global(), 0, NULL);
}

void Download::onPause() {
  uv_timer_stop(&this->progressTimer);

  if (this->callbackPause) {
    (*(this->callbackPause))->Call(Context::GetCurrent()->Global(), 0, NULL);
  }

}

void Download::onStop() {
  uv_timer_stop(&this->progressTimer);

  if (this->callbackStop) {
    (*(this->callbackStop))->Call(Context::GetCurrent()->Global(), 0, NULL);
  }
}

void Download::onComplete() {
  uv_timer_stop(&this->progressTimer);

  if (this->callbackComplete) {
    HandleScope scope;
    DownloadHandle* info = getDownloadHandle(this->session, this->gid);
    assert(info);
    const unsigned argc = 1;
    Local<Value> argv[argc] = {
        Local<Value>::New(String::New(info->getDir().c_str()))
    };
    deleteDownloadHandle(info);
    (*(this->callbackComplete))->Call(Context::GetCurrent()->Global(), argc, argv);
  }
}

void Download::onError() {
  uv_timer_stop(&this->progressTimer);

  if (this->callbackError) {
    HandleScope scope;
    DownloadHandle* info = getDownloadHandle(this->session, this->gid);
    assert(info);
    const unsigned argc = 1;
    Local<Value> argv[argc] = { Local<Value>::New(Number::New(info->getErrorCode())) };
    deleteDownloadHandle(info);
    (*(this->callbackError))->Call(Context::GetCurrent()->Global(), argc, argv);
  }
}

void Download::onProgress() {
  if (this->callbackProgress) {
    HandleScope scope;
    DownloadHandle* info = getDownloadHandle(this->session, this->gid);
    assert(info);
    const unsigned argc = 3;
    Local<Value> argv[argc] = {
        Local<Value>::New(Number::New(info->getTotalLength())),
        Local<Value>::New(Number::New(info->getCompletedLength())),
        Local<Value>::New(Number::New(info->getDownloadSpeed())),
    };
    deleteDownloadHandle(info);
    (*(this->callbackProgress))->Call(Context::GetCurrent()->Global(), argc, argv);
  }
}