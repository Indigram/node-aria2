#include <chrono>
#include <unordered_map>
#include <assert.h>
#include <aria2/aria2.h>
#include <node.h>
#include <unistd.h>

#include "command/shutdown.h"
#include "command/config.h"
#include "command/add.h"
#include "command.h"
#include "event/complete.h"
#include "event/error.h"
#include "event/pause.h"
#include "event/progress.h"
#include "event/start.h"
#include "event/stop.h"
#include "event.h"
#include "download.h"
#include "queue.h"
#include "aria2.h"

using namespace std;
using namespace std::chrono;
using namespace v8;
using namespace aria2;

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

  assert(libraryInit() == 0);
}

void Aria2::Deinit() {
  assert(libraryDeinit() == 0);
}

Handle<Value> Aria2::NewInstance() {
  HandleScope scope;
  Local<Object> instance = constructor->NewInstance(0, NULL);
  return scope.Close(instance);
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

  Aria2* aria = ObjectWrap::Unwrap < Aria2 > (args.This());
  aria->commands->push(unique_ptr<Command>(new command::Config(move(options))));

  return scope.Close(Undefined());
}

Handle<Value> Aria2::Download(const Arguments& args) {
  HandleScope scope;

  if (args.Length() < 1) {
    ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
    return scope.Close(Undefined());
  }

  if (!args[0]->IsString()) {
    ThrowException(Exception::TypeError(String::New("Wrong arguments")));
    return scope.Close(Undefined());
  }

  uuid_t uuid;
  uuid_generate_random(uuid);
  char uid[37];
  uuid_unparse(uuid, uid);

  Aria2* aria = ObjectWrap::Unwrap < Aria2 > (args.This());
  Handle<Value> download = Download::NewInstance(String::New(uid), External::New(aria->commands));

  aria->downloads.insert(make_pair(
      string(uid),
      Persistent<Object>::New(download->ToObject())
  ));

  String::Utf8Value uri(args[0]->ToString());
  aria->commands->push(unique_ptr<Command>(new command::Add(*uri, string(uid))));

  return scope.Close(download);
}

void Aria2::DownloadWork(void* data) {
  Aria2* self = static_cast<Aria2*>(data);
  Queue<Command>* commands = self->commands;
  Queue<Event>* events = self->events;
  unordered_map<A2Gid, string>* idmap = &self->idmap;

  Session* session;
  SessionConfig config;
  config.downloadEventCallback = EventCallback;
  config.useSignalHandler = false;
  config.keepRunning = true;
  config.userData = self;
  session = aria2::sessionNew(aria2::KeyVals(), config);
  steady_clock::time_point start = steady_clock::now();

  for (; ;) {
    while (!commands->empty()) {
      unique_ptr<Command> command = commands->pop();
      command->execute(session, idmap);
    }

    if (1 != aria2::run(session, aria2::RUN_ONCE)) {
      break;
    }

    steady_clock::time_point now = steady_clock::now();
    if (900 <= duration_cast<milliseconds>(now - start).count()) {
      start = now;
      vector<A2Gid> gids = getActiveDownload(session);
      for (const auto& gid : gids) {
        DownloadHandle* info = getDownloadHandle(session, gid);
        if (info) {
          events->push(unique_ptr<Event>(new event::Progress(
              idmap->at(gid),
              info->getTotalLength(),
              info->getCompletedLength(),
              info->getDownloadSpeed()
          )));
          deleteDownloadHandle(info);
        }
      }
    }
  }

  aria2::sessionFinal(session);
}

int Aria2::EventCallback(Session* session, DownloadEvent event, A2Gid gid, void* arg) {
  Aria2* aria = static_cast<Aria2*>(arg);
  Queue<Event>* events = aria->events;
  unordered_map<aria2::A2Gid, std::string>* idmap = &aria->idmap;
  DownloadHandle* info;
  switch (event) {
    case EVENT_ON_DOWNLOAD_START:
      events->push(unique_ptr<Event>(new event::Start(idmap->at(gid))));
      break;
    case EVENT_ON_DOWNLOAD_PAUSE:
      events->push(unique_ptr<Event>(new event::Pause(idmap->at(gid))));
      break;
    case EVENT_ON_DOWNLOAD_STOP:
      events->push(unique_ptr<Event>(new event::Stop(idmap->at(gid))));
      break;
    case EVENT_ON_DOWNLOAD_COMPLETE:
      info = getDownloadHandle(session, gid);
      assert(info);
      events->push(unique_ptr<Event>(new event::Complete(idmap->at(gid), info->getDir().c_str())));
      deleteDownloadHandle(info);
      break;
    case EVENT_ON_DOWNLOAD_ERROR:
      info = getDownloadHandle(session, gid);
      assert(info);
      events->push(unique_ptr<Event>(new event::Error(idmap->at(gid), info->getErrorCode())));
      deleteDownloadHandle(info);
      break;
    case EVENT_ON_BT_DOWNLOAD_COMPLETE:
      break;
  }
  return 0;
}

void Aria2::OnEvent(uv_async_t* handle, int status) {
  Aria2* aria = static_cast<Aria2*>(handle->data);
  Queue<Event>* events = aria->events;
  while (!events->empty()) {
    unique_ptr<Event> event = events->pop();
    event->on(&aria->downloads);
  }
}

Aria2::Aria2() {
}

Aria2::~Aria2() {
}

void Aria2::init() {
  commands = new Queue<Command>();
  events = new Queue<Event>(OnEvent, this);

  uv_thread_create(&downloadWorker, DownloadWork, this);
}

void Aria2::deinit() {
  commands->push(unique_ptr<Command>(new command::Shutdown()));
  uv_thread_join(&downloadWorker);

  delete events;
  delete commands;
}