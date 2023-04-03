#ifndef TENACITAS_LIB_ASYNC_INTERNAL_EVT_CONNECTED_H
#define TENACITAS_LIB_ASYNC_INTERNAL_EVT_CONNECTED_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <iostream>

#include <tenacitas.lib.async/internal/typ/connection.h>
#include <tenacitas.lib.async/typ/event_id.h>

namespace tenacitas::lib::async::internal::evt {

struct connected final {

  connected(typ::connection::ptr p_connection) : connection(p_connection) {}

  connected() = default;
  connected(const connected &) = default;
  connected(connected &&) = default;
  ~connected() = default;

  connected &operator=(const connected &) = default;
  connected &operator=(connected &&) = default;

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const connected &p_connected) {
    p_out << '{' << p_connected.connection->channel_id << ','
          << p_connected.connection->connection_id << '}';
    return p_out;
  }

  typ::connection::ptr connection;

  static constexpr async::typ::event_id id{1678127779338};
};

} // namespace tenacitas::lib::async::internal::evt

#endif
