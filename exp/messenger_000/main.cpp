/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

/// \example messenger_000

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <iostream>
#include <mutex>

#include <tenacitas.lib.async/alg/messenger.h>
#include <tenacitas.lib.async/alg/sleeping_loop.h>
#include <tenacitas.lib.async/typ/channel_id.h>
#include <tenacitas.lib.async/typ/codec.h>
#include <tenacitas.lib.async/typ/deserializer.h>
#include <tenacitas.lib.async/typ/serializer.h>
#include <tenacitas.lib.log/alg/logger.h>
#include <tenacitas.lib.number/typ/id.h>

using namespace tenacitas::lib;
using namespace std::chrono_literals;

namespace messenger_000 {

namespace typ {

using sensor_id = uint16_t;
using value = float;

} // namespace typ

namespace evt {

struct exit {
  friend std::ostream &operator<<(std::ostream &p_out, const exit &) {
    p_out << "exit";
    return p_out;
  }

  static constexpr uint64_t id{1670796516900};
};

struct dummy {
  friend std::ostream &operator<<(std::ostream &p_out, const dummy &) {
    p_out << "dummy";
    return p_out;
  }

  static constexpr uint64_t id{1670796516901};
};

struct new_value {
  new_value() = default;
  new_value(typ::sensor_id p_sensor_id, typ::value p_value)
      : sensor_id(p_sensor_id), value(p_value) {}

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const new_value &p_new_value) {
    p_out << "new_value: {" << p_new_value.sensor_id << ',' << p_new_value.value
          << '}';
    return p_out;
  }

  typ::sensor_id sensor_id;
  typ::value value;
  static constexpr uint64_t id{1670796516902};
};
} // namespace evt

namespace ipc {

struct channel_0 {

  static constexpr async::typ::channel_id id{0};

  channel_0() { m_sleeping_loop.start(); }

  ~channel_0() { m_sleeping_loop.stop(); }

  async::typ::message receive() {
    std::this_thread::sleep_for(1s);
    std::string _str{m_id_str + "|" + std::to_string(m_value)};
    async::typ::message _msg{(const std::byte *)(_str.begin().base()),
                             (const std::byte *)(_str.end().base())};
    return _msg;
  }

  void send(const async::typ::message &) {}

private:
  typ::sensor_id m_id{45};
  std::string m_id_str{std::to_string(m_id)};
  typ::value m_value{1};
  async::alg::sleeping_loop m_sleeping_loop{[this]() { m_value += 0.5; }, 1s};
};

struct deserialize_new_value_channel_0 {
  std::optional<evt::new_value>
  operator()(const async::typ::message &p_message) {
    auto _ite{std::find(p_message.begin(), p_message.end(), std::byte{'|'})};
    std::string _s1{(const char *)(p_message.begin().base()),
                    (const char *)(_ite.base())};
    std::string _s2{(const char *)((++_ite).base()),
                    (const char *)(p_message.end().base())};
    evt::new_value _new_value{static_cast<typ::sensor_id>(std::stoi(_s1)),
                              std::stof(_s2)};
    return _new_value;
  }
};

struct serialize_new_value_channel_0 {
  std::optional<async::typ::message> operator()(const evt::new_value &) {
    return {};
  }
};

} // namespace ipc

} // namespace messenger_000

int main() {
  using namespace messenger_000;
  log::alg::set_info_level();

  ipc::channel_0 _channel_0;

  using codec = async::typ::codec<ipc::channel_0::id, evt::new_value,
                                  ipc::serialize_new_value_channel_0,
                                  ipc::deserialize_new_value_channel_0>;

  //  using channel_0_codec = async::typ::channel<ipc::channel_0::id, ;

  using messenger = async::alg::messenger<codec>;

  messenger::ptr _messenger{messenger::create()};

  std::condition_variable _cond_exit;

  // subscriptions
  _messenger->subscribe<evt::exit>(
      [&_cond_exit](auto) { _cond_exit.notify_one(); });

  _messenger->subscribe<evt::dummy>([_messenger](auto) {
    TNCT_LOG_TST("sleeping for 10 secs");
    std::this_thread::sleep_for(10s);
    TNCT_LOG_TST("publishing exit");
    _messenger->publish<evt::exit>();
  });

  _messenger->subscribe<evt::new_value>(
      [](auto p_event) { TNCT_LOG_TST(p_event); });

  // publishing
  _messenger->publish<evt::dummy>();

  _messenger->publish<async::evt::new_connection>(
      ipc::channel_0::id, async::typ::connection_id{0},
      [&_channel_0](const async::typ::message &p_message) -> void {
        _channel_0.send(p_message);
      },
      [&_channel_0]() -> async::typ::message { return _channel_0.receive(); });

  // waiting for messenger_000::evt::exit event
  {

    std::mutex _mutex_exit;

    TNCT_LOG_TST("waiting for signal to exit");
    std::unique_lock<std::mutex> _lock{_mutex_exit};
    _cond_exit.wait(_lock);
  }
  TNCT_LOG_TST("exiting");
}
