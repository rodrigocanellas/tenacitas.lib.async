#ifndef TENACITAS_LIB_ASYNC_EVT_NEW_CONNECTION_H
#define TENACITAS_LIB_ASYNC_EVT_NEW_CONNECTION_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <iostream>

#include <tenacitas.lib.async/typ/channel_id.h>
#include <tenacitas.lib.async/typ/connection_id.h>
#include <tenacitas.lib.async/typ/event_id.h>
#include <tenacitas.lib.async/typ/receiver.h>
#include <tenacitas.lib.async/typ/sender.h>

namespace tenacitas::lib::async::evt {

struct new_connection {
  new_connection() = default;
  new_connection(const new_connection &) = default;
  new_connection(new_connection &&) = default;
  new_connection &operator=(const new_connection &) = default;
  new_connection &operator=(new_connection &&) = default;
  ~new_connection() = default;

  new_connection(typ::channel_id p_channel_id,
                 typ::connection_id p_connection_id, typ::sender p_send,
                 typ::receiver p_receive)
      : channel_id(p_channel_id), connection_id(p_connection_id), send(p_send),
        receive(p_receive) {}

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const new_connection &p_new_connection) {
    p_out << "new_connection {channel = " << p_new_connection.channel_id
          << ", connection = " << p_new_connection.connection_id << '}';
    return p_out;
  }

  typ::channel_id channel_id;
  typ::connection_id connection_id;
  typ::sender send;
  typ::receiver receive;

  static constexpr typ::event_id id{1669763783652};
};

} // namespace tenacitas::lib::async::evt

#endif
