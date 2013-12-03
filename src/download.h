#ifndef _DOWNLOAD_H
#define _DOWNLOAD_H

#include <string>
#include <aria2/aria2.h>
#include <node.h>

class Download : public node::ObjectWrap {
public:
  static void Init();
  static v8::Handle<v8::Value> NewInstance(const v8::Arguments& args, const v8::Handle<v8::Value> session);
  Download(aria2::Session* session);
  virtual ~Download();
  void onEvent(aria2::DownloadEvent event);
  aria2::A2Gid getGid();

private:
  static v8::Persistent<v8::Function> constructor;
  static v8::Handle<v8::Value> New(const v8::Arguments& args);
  static v8::Handle<v8::Value> On(const v8::Arguments& args);
  static v8::Handle<v8::Value> Start(const v8::Arguments& args);
  static v8::Handle<v8::Value> Pause(const v8::Arguments& args);
  static v8::Handle<v8::Value> Remove(const v8::Arguments& args);
  static void ProgressWork(uv_timer_t* handle, int status);
  static void ReplaceFunction(v8::Persistent<v8::Function>** destination, v8::Handle<v8::Value> source);
  static void DisposeFunction(v8::Persistent<v8::Function>** function);
  void addUri(std::string uri);
  void start();
  void pause();
  void remove();

  void onStart();
  void onPause();
  void onStop();
  void onComplete();
  void onError();
  void onProgress();

  v8::Persistent<v8::Function>* callbackStart;
  v8::Persistent<v8::Function>* callbackPause;
  v8::Persistent<v8::Function>* callbackStop;
  v8::Persistent<v8::Function>* callbackComplete;
  v8::Persistent<v8::Function>* callbackError;
  v8::Persistent<v8::Function>* callbackProgress;
  uv_timer_t progressTimer;
  aria2::Session* session;
  aria2::A2Gid gid;
  std::string uri;

};

#endif