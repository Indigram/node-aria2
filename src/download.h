#ifndef _DOWNLOAD_H
#define _DOWNLOAD_H

#include <string>
#include <node.h>
#include "queue.h"
#include "command.h"

class Download : public node::ObjectWrap {
public:
  static void Init();
  static v8::Handle<v8::Value> NewInstance(const v8::Handle<v8::Value> uid, const v8::Handle<v8::Value> commands);
  Download(std::string uid, Queue<Command>* commands);
  virtual ~Download();
  void processOnComplete(std::string path);
  void processOnError(int code);
  void processOnPause();
  void processOnProgress(int total, int completed, int speed);
  void processOnStop();
  void processOnStart();

private:
  static v8::Persistent<v8::Function> constructor;
  static v8::Handle<v8::Value> New(const v8::Arguments& args);
  static v8::Handle<v8::Value> On(const v8::Arguments& args);
  static v8::Handle<v8::Value> Start(const v8::Arguments& args);
  static void ReplaceFunction(v8::Persistent<v8::Function>** destination, v8::Handle<v8::Value> source);
  static void DisposeFunction(v8::Persistent<v8::Function>** function);

  std::string uid;
  Queue<Command>* commands;
  v8::Persistent<v8::Function>* callbackOnStart;
  v8::Persistent<v8::Function>* callbackOnPause;
  v8::Persistent<v8::Function>* callbackOnStop;
  v8::Persistent<v8::Function>* callbackOnComplete;
  v8::Persistent<v8::Function>* callbackOnError;
  v8::Persistent<v8::Function>* callbackOnProgress;
};

#endif