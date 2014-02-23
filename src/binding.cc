#include <node.h>
#include "aria2.h"
#include "download.h"

using namespace v8;
using namespace node;

void Deinit(uv_signal_t* signal, int);

void Init(Handle<Object> exports, Handle<Value> module) {
  Download::Init();
  Aria2::Init();

  Persistent<Value>* aria = new Persistent<Value>();
  *aria = Persistent<Value>::New(Aria2::NewInstance());
  ObjectWrap::Unwrap<Aria2>((*aria)->ToObject())->init();

  uv_signal_t* signal = new uv_signal_t();
  signal->data = aria;
  uv_signal_init(uv_default_loop(), signal);
  uv_signal_start(signal, Deinit, SIGINT);

  module->ToObject()->Set(String::New("exports"), *aria);
}

void Deinit(uv_signal_t* signal, int) {
  uv_signal_stop(signal);
  delete signal;

  Persistent<Value>* aria = static_cast<Persistent<Value>*>(signal->data);
  ObjectWrap::Unwrap<Aria2>((*aria)->ToObject())->deinit();
  delete aria;

  Aria2::Deinit();
}

NODE_MODULE(node_aria2, Init)