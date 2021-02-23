//
// Created by w1ckedente on 22.02.2021.
//

#ifndef MULTI_THREADING_HASH_DATA_HPP
#define MULTI_THREADING_HASH_DATA_HPP

#include <string>

namespace hasher::calculating {
struct hash_data {
  std::string src;
  std::string sha256;
};
}  // namespace hasher::calculating

#endif  // MULTI_THREADING_HASH_DATA_HPP
