//
// Created by w1ckedente on 17.02.2021.
//

#ifndef MULTI_THREADING_RANDOM_HASH_GENERATOR_HPP
#define MULTI_THREADING_RANDOM_HASH_GENERATOR_HPP

#include <picosha2.h>

#include <calculating/hash_data.hpp>
#include <string>
#include <type_traits>
#include <utility>

namespace hasher::calculating {
template <typename str_generator_t>
class random_hash_generator {
 public:
  explicit random_hash_generator(str_generator_t str_generator)
      : _str_generator(std::move(str_generator)) {}

  random_hash_generator(const random_hash_generator& other) {
    static_assert(std::is_copy_constructible_v<str_generator_t>,
                  "string generator is not copy constructible");
    _str_generator = other._str_generator;
  }

  random_hash_generator(random_hash_generator&& other) noexcept {
    static_assert(std::is_move_constructible_v<str_generator_t>,
                  "string generator is not move constructible");
    _str_generator = std::move(other._str_generator);
  }

  random_hash_generator& operator=(const random_hash_generator& other) {
    static_assert(std::is_copy_assignable_v<str_generator_t>,
                  "string generator is not copy assignable");
    if (&other == this) return *this;
    _str_generator = other._str_generator;
    return *this;
  }

  random_hash_generator& operator=(random_hash_generator&& other) noexcept {
    static_assert(std::is_move_assignable_v<str_generator_t>,
                  "string generator is not move assignable");
    if (&other == this) return *this;
    _str_generator = std::move(other._str_generator);
    return *this;
  }

  ~random_hash_generator() = default;

  hash_data operator()() {
    std::string src = std::move(_str_generator());
    std::string sha256;
    picosha2::hash256_hex_string(src, sha256);
    return hash_data{src, sha256};
  }

 private:
  str_generator_t _str_generator;
};
}  // namespace hasher::calculating

#endif  // MULTI_THREADING_RANDOM_HASH_GENERATOR_HPP
