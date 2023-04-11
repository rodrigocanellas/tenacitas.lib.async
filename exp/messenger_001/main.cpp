/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

/// \example messenger_001

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <iostream>
#include <mutex>

#include <tenacitas.lib.async/alg/messenger.h>
#include <tenacitas.lib.async/alg/sleeping_loop.h>
#include <tenacitas.lib.log/alg/logger.h>

using namespace tenacitas;
using namespace tenacitas::lib;
using namespace std::chrono_literals;

// async::alg::messenger::ptr g_messenger{async::alg::messenger::create()};

namespace typ {
using sensor_id = uint16_t;
using pressure = float;
} // namespace typ

namespace evt {

struct exit {
  friend std::ostream &operator<<(std::ostream &p_out, const exit &) {
    p_out << "exit";
    return p_out;
  }

  static constexpr uint64_t id{1671058347272};
};

struct new_pressure {
  new_pressure() = default;
  new_pressure(typ::sensor_id p_sensor_id, typ::pressure p_pressure)
      : sensor_id(p_sensor_id), pressure(p_pressure) {}

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const new_pressure &p_new_pressure) {
    p_out << "new_pressure: {" << p_new_pressure.sensor_id << ','
          << p_new_pressure.pressure << '}';
    return p_out;
  }

  typ::sensor_id sensor_id;
  typ::pressure pressure;
  static constexpr uint64_t id{1671063524167};
};

} // namespace evt

namespace alg {
void handle_new_pressure(evt::new_pressure &&p_new_pressure) {
  TNCT_LOG_TST(p_new_pressure);
}
} // namespace alg

struct io {

  void send(const async::typ::message &p_msg) {
    std::lock_guard<std::mutex> _lock(m_mutex_msg);
    m_msg = p_msg;
    m_cond_receive.notify_one();
  }

  async::typ::message receive() {
    std::unique_lock<std::mutex> _lock(m_mutex_receive);
    m_cond_receive.wait_for(_lock, 1s, [this]() { return m_stopped.load(); });
    if (m_stopped) {
      TNCT_LOG_TST("m_stopped is  ", m_stopped.load());
      return {};
    }
    if (m_msg.empty()) {
      TNCT_LOG_TST("m_msg is empty");
      return async::typ::message{};
    }
    TNCT_LOG_TST("m_msg is ", async::typ::msg2str(m_msg.begin(), m_msg.end()));
    async::typ::message _aux{m_msg};
    m_msg.clear();
    return _aux;
  }

  ~io() {
    m_stopped = true;
    m_cond_receive.notify_one();
  }

  evt::new_pressure deserialize(async::typ::message &&p_msg) {
    auto _pipe{std::find(p_msg.begin(), p_msg.end(), std::byte{'|'})};

    std::string _s1{reinterpret_cast<const char *>(p_msg.begin().base()),
                    reinterpret_cast<const char *>(_pipe.base())};
    typ::sensor_id _sensor_id{static_cast<typ::sensor_id>(std::stoi(_s1))};

    ++_pipe;
    std::string _s2{reinterpret_cast<const char *>(_pipe.base()),
                    reinterpret_cast<const char *>(p_msg.end().base())};

    typ::pressure _pressure{std::stof(_s2)};

    return evt::new_pressure(_sensor_id, _pressure);
  }

  async::typ::message serialize(const evt::new_pressure p_new_pressure) {
    return serialize(p_new_pressure.sensor_id, p_new_pressure.pressure);
  }

  async::typ::message serialize(typ::sensor_id p_sensor_id,
                                typ::pressure p_pressure) {
    std::string _str{std::to_string(p_sensor_id) + "|" +
                     std::to_string(p_pressure)};
    async::typ::message _msg{(const std::byte *)(_str.begin().base()),
                             (const std::byte *)(_str.end().base())};
    return _msg;
  }

private:
  async::typ::message m_msg;
  std::mutex m_mutex_msg;
  std::condition_variable m_cond_receive;
  std::mutex m_mutex_receive;
  std::atomic_bool m_stopped{false};
};

int main() {
  log::alg::set_debug_level();
  log::alg::set_max_file_name_lenght(16);

  std::condition_variable cond_exit;

  async::alg::messenger::ptr _messenger_1{async::alg::messenger::create(2)};

  async::alg::dispatcher::ptr _dispatcher{async::alg::dispatcher::create()};

  async::alg::messenger::ptr _messenger_2{async::alg::messenger::create(3)};

  TNCT_LOG_TST("creating publishings");
  _dispatcher->subscribe<evt::exit>([&cond_exit](auto) {
    TNCT_LOG_TST("sleeping for 5 secs");
    std::this_thread::sleep_for(5s);
    TNCT_LOG_TST("notifying to exit");
    cond_exit.notify_one();
  });

  TNCT_LOG_TST("creating io");
  auto _io{std::make_shared<io>()};

  auto _receive = [_io]() { return _io->receive(); };
  auto _send = [_io](const async::typ::message &p_msg) { _io->send(p_msg); };
  auto _deserialize = [_io](async::typ::message &&p_msg) -> evt::new_pressure {
    return _io->deserialize(std::move(p_msg));
  };

  TNCT_LOG_TST("publishing new io");
  _messenger_1->publish<async::evt::new_channel>(async::typ::channel_id{1},
                                                 _receive, _send, _deserialize);

  _messenger_2->publish<async::evt::new_channel>(async::typ::channel_id{1},
                                                 _receive, _send, _deserialize);

  TNCT_LOG_TST("subscribing for new_pressure in messenger 1");
  _messenger_1->subscribe<evt::new_pressure>(alg::handle_new_pressure);

  //  TNCT_LOG_TST("publishing new_pressure in messenger 2");
  //  _messenger_2->publish<evt::new_pressure>(typ::sensor_id{35},
  //                                           typ::pressure{25.8});

  // waiting for evt::exit event
  _dispatcher->publish<evt::exit>();
  {

    std::mutex mutex_exit;

    TNCT_LOG_TST("waiting for signal to exit");
    std::unique_lock<std::mutex> _lock{mutex_exit};
    cond_exit.wait(_lock);
  }

  TNCT_LOG_TST("exiting");
}
