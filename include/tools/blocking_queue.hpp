//
// Created by w1ckedente on 23.02.2021.
//

#ifndef MULTI_THREADING_BLOCKING_QUEUE_HPP
#define MULTI_THREADING_BLOCKING_QUEUE_HPP

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <utility>

template <typename T>
class blocking_queue {
 public:
  blocking_queue() : _available(true) {}

  blocking_queue(const blocking_queue& other) = delete;

  blocking_queue(blocking_queue&& other) noexcept
      : _available(std::move(other._available)),
        _queue_container(std::move(other._queue_container)),
        _condition(std::move(other._condition)),
        _mutex(std::move(other._mutex)) {}

  blocking_queue& operator=(const blocking_queue& other) = delete;

  blocking_queue& operator=(blocking_queue&& other) noexcept {
    if (&other == this) return *this;
    stop();
    std::queue<T> clear_queue;
    std::swap(_queue_container, clear_queue);
    _available = std::move(other._available);
    _mutex = std::move(other._mutex);
    _condition = std::move(other._condition);

    return *this;
  }

  ~blocking_queue() { stop(); }

  [[nodiscard]] bool push(const T& value) {
    if (!_available) return false;
    {
      std::unique_lock<std::mutex> lock(_mutex);
      _queue_container.push(value);
    }
    _condition.notify_one();
    return true;
  }

  [[nodiscard]] bool push(T&& value) {
    if (!_available) return false;
    {
      std::unique_lock<std::mutex> lock(_mutex);
      _queue_container.push(std::move(value));
    }
    _condition.notify_one();
    return true;
  }

  template <typename... args_t>
  [[nodiscard]] bool emplace(args_t&&... args) {
    if (!_available) return false;
    {
      std::unique_lock<std::mutex> lock(_mutex);
      _queue_container.emplace(std::forward<args_t>(args)...);
    }
    _condition.notify_one();
    return true;
  }

  T pop() {
    std::unique_lock<std::mutex> lock(_mutex);
    _condition.wait(lock,
                    [=] { return _available || !_queue_container.empty(); });
    T ret_value(std::move(_queue_container.back()));
    _queue_container.pop();
    return ret_value;
  }

  void stop() {
    {
      std::unique_lock<std::mutex> lock(_mutex);
      _available = false;
    }
    _condition.notify_all();
  }

  bool empty() {
    std::unique_lock<std::mutex> lock(_mutex);
    return _queue_container.empty();
  }

 private:
  std::atomic_bool _available;
  std::queue<T> _queue_container;
  std::mutex _mutex;
  std::condition_variable _condition;
};

#endif  // MULTI_THREADING_BLOCKING_QUEUE_HPP
