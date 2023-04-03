#ifndef TENACITAS_LIB_ASYNC_INTERNAL_ALG_IN_MESSENGER_H
#define TENACITAS_LIB_ASYNC_INTERNAL_ALG_IN_MESSENGER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <thread>
#include <tuple>
#include <type_traits>
#include <typeindex>
#include <utility>

#include <tenacitas.lib.async/alg/dispatcher.h>
#include <tenacitas.lib.async/internal/evt/connected.h>
#include <tenacitas.lib.async/internal/typ/connection.h>
#include <tenacitas.lib.async/typ/channel_id.h>
#include <tenacitas.lib.async/typ/message.h>
#include <tenacitas.lib.log/alg/logger.h>

namespace tenacitas::lib::async::internal::alg {

template <typename... t_codecs> struct in_messenger {

  in_messenger() = delete;
  in_messenger(const in_messenger &) = default;
  in_messenger(in_messenger &&) = default;
  ~in_messenger() = default;

  in_messenger &operator=(const in_messenger &) = default;
  in_messenger &operator=(in_messenger &&) = default;

  in_messenger(async::alg::dispatcher &p_dispatcher,
               uint8_t p_num_new_msg_subscribers = 1);

  void stop();

private:
  using codecs = std::tuple<t_codecs...>;

  struct in_message final {
    in_message(async::typ::channel_id p_channel_id,
               async::typ::message &&p_message)
        : channel_id(p_channel_id), message(p_message) {}

    in_message() = default;
    in_message(const in_message &) = default;
    in_message(in_message &&) = default;
    ~in_message() = default;

    in_message &operator=(const in_message &) = default;
    in_message &operator=(in_message &&) = default;

    friend std::ostream &operator<<(std::ostream &p_out,
                                    const in_message &p_new_msg) {
      p_out << "new in msg: channel_id " << p_new_msg.channel_id << ", msg = '"
            << async::typ::msg2str(p_new_msg.message.begin(),
                                   p_new_msg.message.end())
            << '\'';
      return p_out;
    }

    async::typ::channel_id channel_id;
    async::typ::message message;
    static constexpr async::typ::event_id id{1669764065120};
  };

  struct receive_loop final {

    receive_loop(typ::connection::ptr p_connection)
        : connection(p_connection) {}

    receive_loop() = default;
    receive_loop(const receive_loop &) = default;
    receive_loop(receive_loop &&) = default;
    ~receive_loop() = default;

    receive_loop &operator=(const receive_loop &) = default;
    receive_loop &operator=(receive_loop &&) = default;

    friend std::ostream &
    operator<<(std::ostream &p_out,
               const receive_loop &p_new_communication_handling) {
      p_out << "new_connection_publishing "
            << *p_new_communication_handling.connection;
      return p_out;
    }
    typ::connection::ptr connection;
    static constexpr async::typ::event_id id{1671493462298};
  };

private:
  void on_connected(evt::connected &&p_connected);

  void on_receive_loop(receive_loop &&p_receive_loop);

  void on_in_message(in_message &&p_new_message);

  template <size_t index = 0>
  void deserialize(async::typ::channel_id p_channel_id,
                   const async::typ::message &p_message);

private:
  async::alg::dispatcher &m_dispatcher;
  number::typ::id m_id;
  std::atomic_bool m_stop{false};
};

// IMPLEMENTATION ###########################################################

template <typename... t_codecs>
in_messenger<t_codecs...>::in_messenger(async::alg::dispatcher &p_dispatcher,
                                        uint8_t p_num_new_msg_subscribers)
    : m_dispatcher(p_dispatcher) {
  m_dispatcher.subscribe<evt::connected>(
      [this](auto p_event) { on_connected(std::move(p_event)); });

  auto _in_msg_queue_id =
      m_dispatcher.add_queue<in_message>(async::typ::priority::highest);

  auto _new_msg_subscriber = [this](in_message &&p_event) {
    on_in_message(std::move(p_event));
  };

  auto _new_msg_subscriber_factory = [&]() { return _new_msg_subscriber; };

  m_dispatcher.subscribe<in_message>(
      _in_msg_queue_id, p_num_new_msg_subscribers, _new_msg_subscriber_factory);
}

template <typename... t_codecs> inline void in_messenger<t_codecs...>::stop() {
  m_stop = true;
}

template <typename... t_codecs>
void in_messenger<t_codecs...>::on_connected(evt::connected &&p_connected) {

  auto _receive_loop_queue =
      m_dispatcher.add_queue<receive_loop>(async::typ::priority::highest);

  m_dispatcher.subscribe<receive_loop>(
      _receive_loop_queue,
      [this](auto p_event) { on_receive_loop(std::move(p_event)); });

  m_dispatcher.publish_to_queue<receive_loop>(_receive_loop_queue,
                                              p_connected.connection);
}

template <typename... t_codecs>
void in_messenger<t_codecs...>::on_receive_loop(receive_loop &&p_receive_loop) {

#ifdef TENACITAS_LOG
  TNCT_LOG_DEB(m_id, " - ", p_receive_loop);
#endif

  typ::connection::ptr _connection{p_receive_loop.connection};

  async::typ::channel_id _channel_id{_connection->channel_id};

  while (true) {
    if (m_stop) {
#ifdef TENACITAS_LOG
      TNCT_LOG_DEB(m_id, " - breaking because stopped");
#endif
      break;
    }

    std::future<std::optional<async::typ::message>> _future =
        std::async([&] { return _connection->receive(); });

    std::future_status _status{_future.wait_for(5s)};

    if (m_stop) {
#ifdef TENACITAS_LOG
      TNCT_LOG_DEB(m_id, " - breaking because stopped");
#endif
      break;
    }

    if (_status != std::future_status::ready) {
#ifdef TENACITAS_LOG
      TNCT_LOG_DEB(
          m_id, " - ", *_connection, " - nothing received, future status = ",
          (_status == std::future_status::deferred ? "deferred" : "timeout"));
#endif
      continue;
    }

    std::optional<async::typ::message> _maybe{_future.get()};

    if (!_maybe) {
#ifdef TENACITAS_LOG
      TNCT_LOG_DEB(m_id, " - no message");
#endif
      continue;
    }

    async::typ::message _message{std::move(_maybe.value())};

    if (_message.empty()) {
#ifdef TENACITAS_LOG
      TNCT_LOG_DEB(m_id, " - message is empty");
#endif
      continue;
    }

#ifdef TENACITAS_LOG
    TNCT_LOG_DEB(m_id, " - ", *_connection, " - message received = ",
                 async::typ::msg2str(_message.begin(), _message.end()));
#endif

    m_dispatcher.publish<in_message>(_channel_id, std::move(_message));
  }
#ifdef TENACITAS_LOG
  TNCT_LOG_DEB(m_id, " - channel ", _channel_id, " - leaving on_new_channel");
#endif
}

template <typename... t_codecs>
inline void
in_messenger<t_codecs...>::on_in_message(in_message &&p_new_message) {
#ifdef TENACITAS_LOG
  TNCT_LOG_DEB(p_new_message);
#endif
  deserialize(p_new_message.channel_id, p_new_message.message);
}

template <typename... t_codecs>
template <size_t index>
void in_messenger<t_codecs...>::deserialize(
    async::typ::channel_id p_channel_id, const async::typ::message &p_message) {

  using codec = std::tuple_element_t<index, codecs>;

  if (codec::channel_id == p_channel_id) {
    using event = typename codec::event;
    std::optional<event> _maybe{typename codec::deserializer()(p_message)};
    if (_maybe) {
      m_dispatcher.publish<event>(_maybe.value());
      return;
    }
  }

  if constexpr (index + 1 != sizeof...(t_codecs)) {
    deserialize<index + 1>(p_channel_id, p_message);
  }
}

} // namespace tenacitas::lib::async::internal::alg
#endif
