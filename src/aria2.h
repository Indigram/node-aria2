#ifndef _ARIA2_H
#define _ARIA2_H

#include <unordered_map>
#include <aria2/aria2.h>
#include <node.h>

class Aria2 : public node::ObjectWrap {
public:
  static void Init();
  static v8::Handle<v8::Value> NewInstance();
  Aria2();
  virtual ~Aria2();
  void init();
  void destroy();
  void start();
  void stop();


private:
  static v8::Persistent<v8::Function> constructor;
  static v8::Handle<v8::Value> New(const v8::Arguments& args);
  static v8::Handle<v8::Value> Config(const v8::Arguments& args);
  static v8::Handle<v8::Value> Download(const v8::Arguments& args);
  static int EventCallback(aria2::Session* session, aria2::DownloadEvent event, aria2::A2Gid gid, void* arg);
  static void RunWork(uv_timer_t* handle, int status);
  std::unordered_map<aria2::A2Gid, v8::Persistent<v8::Object>> downloads;
  aria2::Session* session;
  uv_timer_t runTimer;

};

#endif