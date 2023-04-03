/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <iostream>
#include <mutex>

#include <tenacitas.lib.async/alg/sleeping_loop.h>

using namespace tenacitas::lib;
using namespace std::chrono_literals;

int main() {

  // interval of execution of the sleeping loop
  const std::chrono::seconds _interval{1s};

  // counter which will be incremented by the function executed in the
  // sleeping loop
  uint16_t _counter{0};

  // max value of the counter
  const uint16_t _max{20};

  // condition variable and mutex to wait for the sleeping loop to finish
  std::condition_variable _cond;
  std::mutex _mutex;

  // function to be executed inside the sleeping loop
  std::function<void()> _function = [&_counter, &_cond]() -> void {
    if (_counter >= _max) {
      _cond.notify_one();
    } else {
      ++_counter;
      std::cout << "counter = " << _counter << std::endl;
    }
  };

  // sleeping loop object
  async::alg::sleeping_loop _sleeping_loop(_function, _interval);

  // starts the loop asynchronously
  _sleeping_loop.start();

  // waits for the function to be executed inside the sleeping loop to notify
  // that the counter reached the maximum value
  std::unique_lock<std::mutex> _lock(_mutex);
  _cond.wait(_lock);
}
