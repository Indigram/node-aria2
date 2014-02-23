#ifndef _ARIA2_H
#define _ARIA2_H

#include <unordered_map>
#include <node.h>
#include <aria2/aria2.h>
#include "queue.h"
#include "command.h"
#include "event.h"

class Aria2 : public node::ObjectWrap {
public:
  static void Init();
  static void Deinit();
  static v8::Handle<v8::Value> NewInstance();
  Aria2();
  virtual ~Aria2();
  void init();
  void deinit();

private:
  static v8::Persistent<v8::Function> constructor;
  static v8::Handle<v8::Value> New(const v8::Arguments& args);
  static v8::Handle<v8::Value> Config(const v8::Arguments& args);
  static v8::Handle<v8::Value> Download(const v8::Arguments& args);
  static void DownloadWork(void* data);
  static int EventCallback(aria2::Session* session, aria2::DownloadEvent event, aria2::A2Gid gid, void* arg);
  static void OnEvent(uv_async_t* handle, int status);

  uv_thread_t downloadWorker;
  Queue<Command>* commands;
  Queue<Event>* events;
  std::unordered_map<std::string, v8::Persistent<v8::Object>> downloads;
  std::unordered_map<aria2::A2Gid, std::string> idmap;
};

#endif