#ifndef TENACITAS_LIB_ASYNC_ALG_MESSENGER_H
#define TENACITAS_LIB_ASYNC_ALG_MESSENGER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <cstdint>
#include <initializer_list>
#include <map>
#include <memory>
#include <optional>
#include <thread>
#include <tuple>
#include <type_traits>
#include <typeindex>
#include <unordered_set>
#include <utility>
#include <vector>

#include <tenacitas.lib.async/alg/dispatcher.h>
#include <tenacitas.lib.async/cpt/concepts.h>
#include <tenacitas.lib.async/evt/new_connection.h>
#include <tenacitas.lib.async/evt/stop_messenger.h>
#include <tenacitas.lib.async/internal/alg/in_messenger.h>
#include <tenacitas.lib.async/internal/alg/out_messenger.h>
#include <tenacitas.lib.async/internal/evt/connected.h>
#include <tenacitas.lib.async/internal/typ/connection.h>
#include <tenacitas.lib.async/internal/typ/connections.h>
#include <tenacitas.lib.async/typ/channel_id.h>
#include <tenacitas.lib.async/typ/connection_id.h>
#include <tenacitas.lib.async/typ/event_id.h>
#include <tenacitas.lib.async/typ/message.h>
#include <tenacitas.lib.log/alg/logger.h>

using namespace std::chrono_literals;

namespace tenacitas::lib::async::alg {

/// \brief Dispatches events based on messages received, and allows events to be
/// sent as messages
///
/// \details Through the definition of a receiver and a deserializer algorithm
/// it is possible to deserialize a message (a byte array) received into a
/// application event, and them to publish to whichever code has subscribed to
/// handle it.
///
/// Internal messages allow other code to define that are interested in an
/// event, which makes \p messenger to subscribe for that event. When any code
/// publishes such an event, \p messenger will use a defined serialized to
/// change the event into a message (byte array), and send to all the objects
/// that declared that are interested in the event.
template <typename... t_codecs> struct messenger : public dispatcher {
  using ptr = std::shared_ptr<messenger>;

  static ptr create();

  messenger(const messenger &) = delete;
  messenger(messenger &&) = delete;

  ~messenger();

  void stop();

  template <cpt::event t_event>
  typ::queue_id
  add_queue(typ::priority p_priority = typ::priority ::medium);

  /// \brief Adds a subscriber to an existing publishing
  ///
  /// \tparam t_event is the type of event that will be handled by the \p
  /// p_subscriber in the existing publishing
  ///
  /// \param p_queue_id is the identifier of the publishing
  ///
  /// \param p_subscriber is the subscriber to be added
  template <cpt::event t_event>
  void subscribe(const typ::queue_id &p_queue_id,
                 typ::subscriber_t<t_event> p_subscriber);

  template <cpt::event t_event>
  typ::queue_id
  subscribe(typ::subscriber_t<t_event> p_subscriber,
            typ::priority p_priority = typ::priority ::medium);

  template <cpt::event t_event>
  void subscribe(const typ::queue_id &p_queue_id, uint16_t p_num_workers,
                 std::function<typ::subscriber_t<t_event>()> p_factory);

  template <cpt::event t_event> bool publish(const t_event &p_event);

  template <typename t_event, typename... t_params>
  bool publish(t_params &&...p_params);

  void clear();

  messenger &operator=(const messenger &) = delete;
  messenger &operator=(messenger &&) = delete;

private:
  using codecs = std::tuple<t_codecs...>;

private:
  messenger(uint8_t p_num_new_msg_subscribers = 1);

private:
  internal::typ::connections m_connections;
  internal::alg::in_messenger<t_codecs...> m_in_messenger;
  internal::alg::out_messenger<t_codecs...> m_out_messenger;
  std::mutex m_mutex_channel;
  number::typ::id m_id;
  std::atomic_bool m_stop{false};
};

// IMPLEMENTATION ###########################################################

template <typename... t_codecs>
typename messenger<t_codecs...>::ptr messenger<t_codecs...>::create() {
  try {
    return ptr(new messenger(1));
  } catch (...) {
#ifdef TENACITAS_LOG
    TNCT_LOG_ERR("could not create 'messenger'");
#endif
  }
  return nullptr;
}

template <typename... t_codecs> inline messenger<t_codecs...>::~messenger() {
#ifdef TENACITAS_LOG
  TNCT_LOG_DEB(m_id, " - destructor");
#endif
  stop();
}

template <typename... t_codecs> void messenger<t_codecs...>::stop() {
  m_in_messenger.stop();
  m_stop = true;
#ifdef TENACITAS_LOG
  TNCT_LOG_DEB(m_id, " - stopping dispatcher");
#endif
  dispatcher::stop();
#ifdef TENACITAS_LOG
  TNCT_LOG_DEB(m_id, " - dispatcher stopped");
#endif
}

template <typename... t_codecs>
template <cpt::event t_event>
inline typ::queue_id
messenger<t_codecs...>::add_queue(typ::priority p_priority) {
  return dispatcher::add_queue<t_event>(p_priority);
}

template <typename... t_codecs>
template <cpt::event t_event>
inline void
messenger<t_codecs...>::subscribe(const typ::queue_id &p_queue_id,
                                  typ::subscriber_t<t_event> p_subscriber) {
  dispatcher::subscribe<t_event>(p_queue_id, p_subscriber);
}

template <typename... t_codecs>
template <cpt::event t_event>
typ::queue_id
messenger<t_codecs...>::subscribe(typ::subscriber_t<t_event> p_subscriber,
                                  typ::priority p_priority) {
  typ::queue_id _id = dispatcher::add_queue<t_event>(p_priority);
  subscribe(_id, p_subscriber);
  return _id;
}

template <typename... t_codecs>
template <cpt::event t_event>
inline void messenger<t_codecs...>::subscribe(
    const typ::queue_id &p_queue_id, uint16_t p_num_workers,
    std::function<typ::subscriber_t<t_event>()> p_factory) {
  dispatcher::subscribe<t_event>(p_queue_id, p_num_workers, p_factory);
}

template <typename... t_codecs>
template <cpt::event t_event>
inline bool messenger<t_codecs...>::publish(const t_event &p_event) {
  bool _res{dispatcher::publish<t_event>(p_event)};

  //    dispatcher::publish<out_message_1<t_event>>(p_event);
  m_out_messenger.send(p_event);
  return _res;
}

template <typename... t_codecs>
template <typename t_event, typename... t_params>
bool messenger<t_codecs...>::publish(t_params &&...p_params) {
  bool _res{dispatcher::publish<t_event>(std::forward<t_params>(p_params)...)};

  t_event _event{std::forward<t_params>(p_params)...};

  m_out_messenger.send(_event);

  return _res;
}

template <typename... t_codecs> inline void messenger<t_codecs...>::clear() {
  dispatcher::clear();
}

template <typename... t_codecs>
messenger<t_codecs...>::messenger(uint8_t p_num_new_msg_subscribers)
    : m_in_messenger(*this, p_num_new_msg_subscribers),
      m_out_messenger(*this, m_connections) {

#ifdef TENACITAS_LOG
  TNCT_LOG_DEB("messenger ", m_id, " created");
#endif

  dispatcher::subscribe<evt::new_connection>([this](auto p_event) -> void {
    internal::typ::connection::ptr _connection{
        internal::typ::connection::create(p_event.channel_id,
                                          p_event.connection_id, p_event.send,
                                          p_event.receive)};
    m_connections.emplace(p_event.channel_id, _connection);

    dispatcher::publish<internal::evt::connected>(_connection);
  });

  dispatcher::subscribe<evt::stop_messenger>([this](auto p_event) {
#ifdef TENACITAS_LOG
    TNCT_LOG_DEB(m_id, " - ", p_event);
#endif
    stop();
  });
}

} // namespace tenacitas::lib::async::alg

#endif
