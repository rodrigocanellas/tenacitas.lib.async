/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

/// \example dispatcher_000

#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <iostream>
#include <mutex>

#include <tenacitas.lib.async/alg/dispatcher.h>
#include <tenacitas.lib.async/alg/sleeping_loop.h>

using namespace tenacitas::lib;
using namespace std::chrono_literals;

// type of value generated
struct temperature {

  temperature(float p_value = 0.0) : value(p_value) {}
  friend std::ostream &operator<<(std::ostream &p_out,
                                  const temperature &p_temp) {
    p_out << "temperature = " << p_temp.value;

    return p_out;
  }
  float value{0.0};
  static constexpr async::typ::event_id id{1669763435650};
};

// event published when all temperatures are received by the subscribers
struct all_handled {
  friend std::ostream &operator<<(std::ostream &p_out, const all_handled &) {
    p_out << "all_handled";
    return p_out;
  }
  static constexpr async::typ::event_id id{1669763447923};
};

// simulates a temperature sensor generating 'temperature' events
struct temperature_sensor {
  // p_dispatcher is responsible for publishing 'temperature' events to be
  // handled by object that subscribed for
  //
  // p_max is the max number of 'temperature' events to be published
  temperature_sensor(async::alg::dispatcher::ptr p_dispatcher, uint16_t p_max)
      : m_dispatcher(p_dispatcher), m_max(p_max),
        m_temperature_generator([this]() { generator(); }, m_interval) {}

  // starts to publish 'temperature' events
  void start() { m_temperature_generator.start(); }

  static constexpr async::typ::event_id id{1669763461862};

private:
  // type for the asynchronous loop that will call the 'generator' method
  using temperature_generator = async::alg::sleeping_loop;

private:
  // function to be executed inside the sleeping loop, that will publish the
  // 'temperature' events
  void generator() {
    if (m_counter >= m_max) {
      if (!m_reported) {
        std::cout << "all temperatures generated" << std::endl;
        m_reported = true;
      }
      return;
    }
    ++m_counter;
    m_temperature.value += 0.2;
    m_dispatcher->publish<temperature>(m_temperature);
  }

private:
  // publish/subscriber
  async::alg::dispatcher::ptr m_dispatcher;

  // counter which will be incremented by the function executed in the
  // sleeping loop
  uint16_t m_counter{0};

  // max number of 'temperature' events to be published
  const uint16_t m_max;

  // temperature to be sent
  temperature m_temperature{23.1};

  // interval of execution of the sleeping loop
  const std::chrono::milliseconds m_interval{500ms};

  // asynchronous loop that will generate temperature at each m_interval
  temperature_generator m_temperature_generator;

  // to avoid writing more than one time that all the temperatures were
  // generated
  bool m_reported{false};
};

// subscriber for a 'temperature' event
struct temperature_subscriber {
  temperature_subscriber(async::alg::dispatcher::ptr p_dispatcher,
                         uint16_t p_max)
      : m_dispatcher(p_dispatcher), m_max(p_max) {}

  void operator()(temperature &&p_temperature) {
    std::cout << ++m_counter << " - " << p_temperature << std::endl;
    std::this_thread::sleep_for(1s);

    if ((m_counter) >= m_max) {
      m_dispatcher->publish<all_handled>();
    }
  }

private:
  async::alg::dispatcher::ptr m_dispatcher;
  uint16_t m_max{0};
  uint16_t m_counter{0};
};

int main() {
  // publisher/subscriber
  async::alg::dispatcher::ptr _dispatcher{async::alg::dispatcher::create()};

  // condition variable and mutex to wait for the sleeping loop to finish
  std::condition_variable _cond;
  std::mutex _mutex;

  // number of temperatures to be generated
  const uint16_t _max{20};

  // adding the subscriber for temperatures sent to a publishing of
  // temperature
  _dispatcher->subscribe<temperature>(
      temperature_subscriber{_dispatcher, _max});

  // declaring the temperature sensor
  temperature_sensor _sensor{_dispatcher, _max};

  // starting the sensor
  _sensor.start();

  // subscriber for the all_handled event signals when all the temperatures
  // were handled
  _dispatcher->subscribe<all_handled>([&](auto) -> void {
    std::cout << "all temperatures handled" << std::endl;
    _cond.notify_one();
    return;
  });

  // waits for the temperature subscriber to publish that the expected number of
  // temperatures was generated
  std::unique_lock<std::mutex> _lock(_mutex);
  _cond.wait(_lock);
}
