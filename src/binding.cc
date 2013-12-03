#include <node.h>
#include "aria2.h"
#include "download.h"

using namespace v8;
using namespace node;

void OnExit(void* arg) {
  Persistent<Value> aria = *(static_cast<Persistent<Value>*>(arg));
  ObjectWrap::Unwrap<Aria2>(aria->ToObject())->destroy();
  delete (Persistent<Value>*)arg;
}

void OnInterruptSignal(uv_signal_t* signal, int) {
  Persistent<Value> aria = *(static_cast<Persistent<Value>*>(signal->data));
  ObjectWrap::Unwrap<Aria2>(aria->ToObject())->stop();
  uv_signal_stop(signal);
  delete signal;
}

void Init(Handle<Object> exports, Handle<Value> module) {
  Aria2::Init();
  Download::Init();

  Persistent<Value>* aria = new Persistent<Value>();
  *aria = Persistent<Value>::New(Aria2::NewInstance());

  ObjectWrap::Unwrap<Aria2>((*aria)->ToObject())->init();
  ObjectWrap::Unwrap<Aria2>((*aria)->ToObject())->start();
  AtExit(OnExit, aria);

  uv_signal_t* signal = new uv_signal_t();
  signal->data = aria;
  uv_signal_init(uv_default_loop(), signal);
  uv_signal_start(signal, OnInterruptSignal, SIGINT);

  module->ToObject()->Set(String::New("exports"), *aria);
}

NODE_MODULE(node_aria2, Init);