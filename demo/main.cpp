#include <iostream>
//#include <logging/async_logger_prototype.hpp>
#include <string>
#include <thread>
#include <tools/blocking_queue.hpp>

int main() {
  blocking_queue<int> test_queue;
  std::vector<std::thread> _workers;
  for (int i = 0; i != 4; ++i) {
    _workers.emplace_back([&] {
      for (int j = 0; j != 4; ++j)
        if (!test_queue.push(i * j)) std::cout << "Error" << std::endl;
    });
  }

  while (!test_queue.empty()) {
    std::cout << test_queue.pop() << std::endl;
  }

  for (auto &worker : _workers) {
    worker.join();
  }
  return 0;
}