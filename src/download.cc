#include <node.h>

#include "download.h"
#include "command/start.h"

using namespace v8;
using namespace std;

Persistent<Function> Download::constructor;

void Download::Init() {
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::New("Download"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  tpl->PrototypeTemplate()->Set(String::New("on"),
      FunctionTemplate::New(On)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::New("start"),
      FunctionTemplate::New(Start)->GetFunction());

  constructor = Persistent<Function>::New(tpl->GetFunction());
}

Handle<Value> Download::NewInstance(const Handle<Value> uid, const Handle<Value> commands) {
  HandleScope scope;

  const int argc = 2;
  Handle<Value> argv[argc] = {uid, commands};
  Local<Object> instance = constructor->NewInstance(argc, argv);

  return scope.Close(instance);
}

Handle<Value> Download::New(const Arguments& args) {
  HandleScope scope;

  if (args.IsConstructCall()) {
    if (args.Length() < 2) {
      ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
      return scope.Close(Undefined());
    }

    if (!args[0]->IsString()) {
      ThrowException(Exception::TypeError(String::New("Wrong arguments")));
    }

    String::Utf8Value uid(args[0]->ToString());
    Queue<Command>* commands = static_cast<Queue<Command>*>(External::Unwrap(args[1]));

    Download* download = new Download(*uid, commands);
    download->Wrap(args.This());

    return args.This();
  } else {
    return scope.Close(constructor->NewInstance(0, NULL));
  }
}

Handle<Value> Download::On(const Arguments& args) {
  HandleScope scope;

  if (args.Length() < 2) {
    ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
    return scope.Close(Undefined());
  }

  if (!args[0]->IsString() || !args[1]->IsFunction()) {
    if (!args[0]->IsString()) {
      ThrowException(Exception::TypeError(String::New("Wrong arguments")));
    }
  }

  Download* download = ObjectWrap::Unwrap < Download > (args.This());
  string event = string(*String::Utf8Value(args[0]->ToString()));

  if (event == "start")
    ReplaceFunction(&download->callbackOnStart, args[1]);
  else if (event == "pause")
    ReplaceFunction(&download->callbackOnPause, args[1]);
  else if (event == "stop")
    ReplaceFunction(&download->callbackOnStop, args[1]);
  else if (event == "complete")
    ReplaceFunction(&download->callbackOnComplete, args[1]);
  else if (event == "error")
    ReplaceFunction(&download->callbackOnError, args[1]);
  else if (event == "progress")
    ReplaceFunction(&download->callbackOnProgress, args[1]);

  return scope.Close(Undefined());
}

Handle<Value> Download::Start(const Arguments& args) {
  HandleScope scope;

  Download* download = ObjectWrap::Unwrap < Download > (args.This());
  download->commands->push(unique_ptr<Command>(new command::Start(download->uid)));

  return scope.Close(Undefined());
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

Download::Download(string uid, Queue<Command>* commands) :
uid(uid),
commands(commands),
callbackOnStart(NULL),
callbackOnPause(NULL),
callbackOnStop(NULL),
callbackOnComplete(NULL),
callbackOnError(NULL),
callbackOnProgress(NULL) {
}

Download::~Download() {
  DisposeFunction(&this->callbackOnStart);
  DisposeFunction(&this->callbackOnPause);
  DisposeFunction(&this->callbackOnStop);
  DisposeFunction(&this->callbackOnComplete);
  DisposeFunction(&this->callbackOnError);
  DisposeFunction(&this->callbackOnProgress);
}

void Download::processOnComplete(string path) {
  if (this->callbackOnComplete) {
    const unsigned argc = 1;
    Local<Value> argv[argc] = {
        Local<Value>::New(String::New(path.c_str()))
    };
    (*(this->callbackOnComplete))->Call(Context::GetCurrent()->Global(), argc, argv);
  }
}

void Download::processOnError(int code) {
  if (this->callbackOnError) {
    const unsigned argc = 1;
    Local<Value> argv[argc] = {
        Local<Value>::New(Number::New(code))
    };
    (*(this->callbackOnError))->Call(Context::GetCurrent()->Global(), argc, argv);
  }
}

void Download::processOnPause() {
  if (this->callbackOnPause) {
    (*(this->callbackOnPause))->Call(Context::GetCurrent()->Global(), 0, NULL);
  }
}

void Download::processOnProgress(int total, int completed, int speed) {
  if (this->callbackOnProgress) {
    const unsigned argc = 3;
    Local<Value> argv[argc] = {
        Local<Value>::New(Number::New(total)),
        Local<Value>::New(Number::New(completed)),
        Local<Value>::New(Number::New(speed)),
    };
    (*(this->callbackOnProgress))->Call(Context::GetCurrent()->Global(), argc, argv);
  }
}

void Download::processOnStop() {
  if (this->callbackOnStop) {
    (*(this->callbackOnStop))->Call(Context::GetCurrent()->Global(), 0, NULL);
  }
}

void Download::processOnStart() {
  if (this->callbackOnStart) {
    (*(this->callbackOnStart))->Call(Context::GetCurrent()->Global(), 0, NULL);
  }
}