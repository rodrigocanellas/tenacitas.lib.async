#ifndef TENACITAS_LIB_ASYNC_INTERNAL_ALG_OUT_MESSENGER_H
#define TENACITAS_LIB_ASYNC_INTERNAL_ALG_OUT_MESSENGER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <iostream>

#include <tenacitas.lib.async/alg/dispatcher.h>
#include <tenacitas.lib.async/cpt/concepts.h>
#include <tenacitas.lib.async/internal/evt/connected.h>
#include <tenacitas.lib.async/internal/typ/connection.h>
#include <tenacitas.lib.async/internal/typ/connections.h>
#include <tenacitas.lib.async/typ/channel_id.h>
#include <tenacitas.lib.async/typ/message.h>
#include <tenacitas.lib.log/alg/logger.h>

namespace tenacitas::lib::async::internal::alg {

template <typename... t_codecs> struct out_messenger {
  out_messenger() = delete;
  out_messenger(const out_messenger &) = default;
  out_messenger(out_messenger &&) = default;
  ~out_messenger() = default;

  out_messenger &operator=(const out_messenger &) = default;
  out_messenger &operator=(out_messenger &&) = default;

  out_messenger(async::alg::dispatcher &p_dispatcher,
                typ::connections &p_connections);

  template <typename t_event> void send(t_event &&p_event);

private:
  using codecs = std::tuple<t_codecs...>;

  struct out_message_1 {
    out_message_1(async::typ::channel_id p_channel_id,
                  async::typ::message &&p_message)
        : channel_id(p_channel_id), message(p_message) {}

    out_message_1() = default;
    out_message_1(const out_message_1 &) = default;
    out_message_1(out_message_1 &&) = default;
    ~out_message_1() = default;

    out_message_1 &operator=(const out_message_1 &) = default;
    out_message_1 &operator=(out_message_1 &&) = default;

    friend std::ostream &operator<<(std::ostream &p_out,
                                    const out_message_1 &p_new_msg) {
      p_out << "new out msg 1: channel_id " << p_new_msg.channel_id
            << ", msg = '"
            << async::typ::msg2str(p_new_msg.message.begin(),
                                   p_new_msg.message.end())
            << '\'';
      return p_out;
    }

    async::typ::channel_id channel_id;
    async::typ::message message;
    static constexpr async::typ::event_id id{1677881960904};
  };

  struct out_message_2 {
    out_message_2(typ::connection::ptr p_connection,
                  async::typ::message &&p_message)
        : connection(p_connection), message(p_message) {}

    out_message_2() = default;
    out_message_2(const out_message_2 &) = default;
    out_message_2(out_message_2 &&) = default;
    ~out_message_2() = default;

    out_message_2 &operator=(const out_message_2 &) = default;
    out_message_2 &operator=(out_message_2 &&) = default;

    friend std::ostream &operator<<(std::ostream &p_out,
                                    const out_message_2 &p_out_msg) {
      p_out << "new out msg: connection " << p_out_msg.connection->connection_id
            << ", msg = '"
            << async::typ::msg2str(p_out_msg.message.begin(),
                                   p_out_msg.message.end())
            << '\'';
      return p_out;
    }

    typ::connection::ptr connection;
    async::typ::message message;
    static constexpr async::typ::event_id id{1677881960904};
  };

private:
  void on_connected(evt::connected &&p_connected);

  template <size_t index = 0, typename t_event>
  void send_event(const t_event &p_event);

private:
  async::alg::dispatcher &m_dispatcher;
  typ::connections &m_connections;
};

// IMPLEMENTATION ###########################################################

template <typename... t_codecs>
out_messenger<t_codecs...>::out_messenger(async::alg::dispatcher &p_dispatcher,
                                          typ::connections &p_connections)
    : m_dispatcher(p_dispatcher), m_connections(p_connections) {

  m_dispatcher.subscribe<evt::connected>(
      [this](auto p_event) { on_connected(std::move(p_event)); });
}

template <typename... t_codecs>
template <typename t_event>
inline void out_messenger<t_codecs...>::send(t_event &&p_event) {
  send_event<0>(std::forward<t_event>(p_event));
}

template <typename... t_codecs>
void out_messenger<t_codecs...>::on_connected(evt::connected &&p_connected) {
  async::typ::queue_id _out_message_queue_2{
      m_dispatcher.add_queue<out_message_2>(async::typ::priority::highest)};

  p_connected.connection->publishing_id = _out_message_queue_2;

  m_dispatcher.subscribe<out_message_2>(
      [this](auto p_event) { p_event.connection->send(p_event.message); });

  m_dispatcher.subscribe<out_message_1>([this](auto p_event) {
    for (typ::connections::value_type &_value : m_connections) {
      if (_value.first == p_event.channel_id) {
        m_dispatcher.publish_to_queue<out_message_2>(
            _value.second->publishing_id,
            out_message_2{_value.second, std::move(p_event.message)});
      }
    }
  });
}

template <typename... t_codecs>
template <size_t index, typename t_event>
void out_messenger<t_codecs...>::send_event(const t_event &p_event) {

  using codec = std::tuple_element_t<index, codecs>;

  using event = typename codec::event;

  if constexpr (std::is_same_v<event, t_event>) {

    using serializer = typename codec::serializer;

    std::optional<async::typ::message> _maybe{serializer()(p_event)};

    if (_maybe) {
      async::typ::channel_id _channel_id{codec::channel_id};
      m_dispatcher.publish<out_message_1>(_channel_id,
                                          std::move(_maybe.value()));
    }
  }

  if constexpr (index + 1 != sizeof...(t_codecs)) {
    send<index + 1, t_event, t_codecs...>(p_event);
  }
}

} // namespace tenacitas::lib::async::internal::alg

#endif
