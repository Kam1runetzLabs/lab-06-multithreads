//
// Created by w1ckedente on 16.02.2021.
//

#ifndef MULTI_THREADING_ASYNC_LOGGER_PROTOTYPE_HPP
#define MULTI_THREADING_ASYNC_LOGGER_PROTOTYPE_HPP

#include <atomic>
#include <boost/log/trivial.hpp>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>

namespace hasher::logging {
template <typename log_message_t, typename log_message_handler_t>
class async_logger_prototype {
 public:
  typedef boost::log::trivial::severity_level log_level;
  typedef log_message_t log_message;

  explicit async_logger_prototype(log_message_handler_t log_message_handler)
      : _log_message_handler(std::move(log_message_handler)), _stopped(false) {}

  async_logger_prototype(const async_logger_prototype &) = delete;
  async_logger_prototype(async_logger_prototype &&) = delete;
  async_logger_prototype &operator=(const async_logger_prototype &) = delete;
  async_logger_prototype &operator=(async_logger_prototype &&) = delete;

  void write_message(log_level level, const log_message_t &message) {}

  ~async_logger_prototype() {
    // todo destructor
  }

 private:
  struct _log_message_meta {
    log_level level;
    log_message_t message;
  };

  log_message_handler_t _log_message_handler;
  std::atomic_bool _stopped;
  std::mutex _mutex;
  std::queue<_log_message_meta> _messages_queue;
  std::thread _logging_worker;
};
}  // namespace hasher::logging

#endif  // MULTI_THREADING_ASYNC_LOGGER_PROTOTYPE_HPP
