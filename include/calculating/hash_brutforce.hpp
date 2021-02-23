//
// Created by w1ckedente on 22.02.2021.
//

#ifndef MULTI_THREADING_HASH_BRUTFORCE_HPP
#define MULTI_THREADING_HASH_BRUTFORCE_HPP

#include <atomic>
#include <boost/thread.hpp>
#include <calculating/random_hash_generator.hpp>
#include <chrono>
#include <logging/logger.hpp>
#include <memory>
#include <string>

namespace hasher::calculating {

template <typename str_generator_t>
class brutforcer {
 public:
  brutforcer(str_generator_t str_generator, const std::string &target,
             std::size_t workers_count);
  brutforcer(const brutforcer &) = delete;
  brutforcer(brutforcer &&) = delete;
  brutforcer &operator=(const brutforcer &) = delete;
  brutforcer &operator=(brutforcer &&) = delete;
  ~brutforcer();

  void start() {
    for (std::size_t i = 0; i != _workers_count; ++i) {
      const auto start_time = std::chrono::high_resolution_clock::now();
      _workers_group.create_thread([this, start_time]() {
        while (!_stopped) {
          // todo processing ctrl + c
          auto hash_data = _hash_generator();
          auto timestamp =
              std::chrono::duration_cast<std::chrono::milliseconds>(
                  std::chrono::high_resolution_clock::now() - start_time)
                  .count();
          if (_matching(hash_data.sha256))
            _logger.write_message(
                logging::logger::log_level::info,
                logging::logger::log_message(boost::this_thread::get_id(),
                                             hash_data, timestamp));
          else
            _logger.write_message(
                logging::logger::log_level::trace,
                logging::logger::log_message(boost::this_thread::get_id(),
                                             hash_data, timestamp));
        }
      });
    }
  }

  void stop();

 private:
  std::atomic_bool _stopped;
  std::size_t _workers_count;
  boost::thread_group _workers_group;
  random_hash_generator<str_generator_t> _hash_generator;
  logging::logger _logger;
  std::string _target;
  bool _matching(const std::string &hash) const;
};
}  // namespace hasher::calculating

#endif  // MULTI_THREADING_HASH_BRUTFORCE_HPP
