#ifndef TENACITAS_LIB_ASYNC_TYP_CONNECTION_H
#define TENACITAS_LIB_ASYNC_TYP_CONNECTION_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <memory>

#include <tenacitas.lib.async/cpt/concepts.h>
#include <tenacitas.lib.async/typ/channel_id.h>
#include <tenacitas.lib.async/typ/connection_id.h>
#include <tenacitas.lib.async/typ/message.h>
#include <tenacitas.lib.async/typ/receiver.h>
#include <tenacitas.lib.async/typ/sender.h>
#include <tenacitas.lib.log/alg/logger.h>

namespace tenacitas::lib::async::internal::typ {

///
/// \brief The connection class
///
struct connection {

  using ptr = std::shared_ptr<connection>;

  connection() = delete;
  connection(const connection &) = delete;
  connection(connection &&) = delete;

  static ptr create(async::typ::channel_id p_channel_id,
                    async::typ::connection_id p_connection_id,
                    async::typ::sender p_send, async::typ::receiver p_receive) {
    try {
      return ptr(
          new connection(p_channel_id, p_connection_id, p_send, p_receive));
    } catch (...) {
#ifdef TENACITAS_LOG
      TNCT_LOG_ERR("it was not possible to create connection for channel ",
                   p_channel_id, ", connection ", p_connection_id);
#endif
    }
    return nullptr;
  }

  ~connection() = default;

  connection &operator=(const connection &) = delete;
  connection &operator=(connection &&) = delete;

  constexpr bool operator<(const connection &p_connection) const {
    if (channel_id < p_connection.channel_id) {
      return true;
    }
    if (channel_id > p_connection.channel_id) {
      return false;
    }
    if (connection_id < p_connection.connection_id) {
      return true;
    }
    return false;
  }

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const connection &p_connection) {
    std::cout << '{' << p_connection.channel_id << ','
              << p_connection.connection_id << '}';
    return p_out;
  }

  async::typ::channel_id channel_id;
  async::typ::connection_id connection_id;
  async::typ::sender send;
  async::typ::receiver receive;
  uint16_t publishing_id;

protected:
  connection(async::typ::channel_id p_channel_id,
             async::typ::connection_id p_connection_id,
             async::typ::sender p_send, async::typ::receiver p_receive)
      : channel_id(p_channel_id), connection_id(p_connection_id), send(p_send),
        receive(p_receive) {}
};

} // namespace tenacitas::lib::async::internal::typ

#endif
