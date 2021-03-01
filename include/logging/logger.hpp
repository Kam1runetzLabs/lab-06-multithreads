//
// Created by w1ckedente on 22.02.2021.
//

#ifndef MULTI_THREADING_LOGGER_HPP
#define MULTI_THREADING_LOGGER_HPP

#include <calculating/hash_data.hpp>
#include <logging/async_logger_prototype.hpp>
#include <nlohmann/json.hpp>
#include <string>

namespace hasher::logging {
struct log_message {
  // thread id
  calculating::hash_data data;
  std::size_t timestamp;
};

class log_message_handler {
 public:
  log_message_handler() = default;

  ~log_message_handler() = default;

  std::string operator()(const log_message &message) {
  }
};

typedef async_logger_prototype<log_message, log_message_handler> logger;
}  // namespace hasher::logging

#endif  // MULTI_THREADING_LOGGER_HPP
