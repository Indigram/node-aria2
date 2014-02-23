#ifndef _QUEUE_H
#define _QUEUE_H

#include <memory>
#include <queue>
#include <mutex>
#include <uv.h>

template<typename T>
class Queue {
public:
  Queue() : onPushed(NULL) {
  }

  Queue(void (*onPushed)(uv_async_t*, int), void* data) : onPushed(onPushed) {
    uv_async_init(uv_default_loop(), &async, onPushed);
    async.data = data;
  }

  virtual ~Queue() {
    if (onPushed) {
      uv_close((uv_handle_t*)&async, NULL);
    }
  }

  void push(std::unique_ptr<T>&& t) {
    std::lock_guard<std::mutex> lock_guard(mutex);
    queue.push(move(t));
    if (onPushed) {
      uv_async_send(&async);
    }
  }

  std::unique_ptr<T> pop() {
    std::lock_guard<std::mutex> lock_guard(mutex);
    std::unique_ptr<T> t = move(queue.front());
    queue.pop();
    return t;
  }

  bool empty() {
    std::lock_guard<std::mutex> lock_guard(mutex);
    return queue.empty();
  }

private:
  std::queue<std::unique_ptr<T>> queue;
  std::mutex mutex;
  uv_async_t async;
  void (*onPushed)(uv_async_t*, int);
};

#endif