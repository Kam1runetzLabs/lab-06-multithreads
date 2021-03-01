//
// Created by w1ckedente on 16.02.2021.
//

#ifndef MULTI_THREADING_ASYNC_LOGGER_PROTOTYPE_HPP
#define MULTI_THREADING_ASYNC_LOGGER_PROTOTYPE_HPP

#include <boost/log/trivial.hpp>
#include <iostream>
#include <thread>
#include <tools/blocking_queue.hpp>
#include <utility>

namespace hasher::logging {
template <typename log_message_t, typename log_message_handler_t>
class async_logger_prototype {
 public:
  typedef boost::log::trivial::severity_level log_level;
  typedef log_message_t log_message;

  explicit async_logger_prototype(log_message_handler_t log_message_handler)
      : _log_message_handler(std::move(log_message_handler)),
        _stopped(false),
        _logging_worker([this] {
          while (!_stopped) {
            // todo logging worker
          }
        }) {}

  async_logger_prototype(const async_logger_prototype &) = delete;
  async_logger_prototype(async_logger_prototype &&) = delete;
  async_logger_prototype &operator=(const async_logger_prototype &) = delete;
  async_logger_prototype &operator=(async_logger_prototype &&) = delete;

  void write_message(log_level level, const log_message_t &message) {
    _messages_queue.push({level, message});
  }

  ~async_logger_prototype() { _messages_queue.stop(); }

  void enable() {
    _stopped = false;
    _logging_worker = std::thread([this] {
      while (!_stopped) {
        // todo logging worker
      }
    });
  }

  void disable() {
    _stopped = true;
    if (_logging_worker.joinable()) _logging_worker.join();
    _messages_queue.stop();
  }

 private:
  struct _log_message_meta {
    log_level level;
    log_message_t message;
  };

  log_message_handler_t _log_message_handler;
  std::atomic_bool _stopped;
  blocking_queue<_log_message_meta> _messages_queue;
  std::thread _logging_worker;
};
}  // namespace hasher::logging

#endif  // MULTI_THREADING_ASYNC_LOGGER_PROTOTYPE_HPP
