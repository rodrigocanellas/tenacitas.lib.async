#ifndef TENACITAS_LIB_ASYNC_TYP_ENDPOINT_H
#define TENACITAS_LIB_ASYNC_TYP_ENDPOINT_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <optional>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <tenacitas.lib.async/typ/message.h>
#include <tenacitas.lib.number/typ/id.h>

namespace tenacitas::lib::async::typ {

struct endpoint {
  template <typename t_int>
  endpoint(std::string_view p_ip, t_int p_port)
      : ip(p_ip), port(static_cast<uint16_t>(p_port)) {}

  endpoint() = default;
  endpoint(const endpoint &) = default;
  endpoint(endpoint &&) = default;
  endpoint &operator=(const endpoint &) = default;
  endpoint &operator=(endpoint &&) = default;

  bool operator==(const endpoint &p_endpoint) const {
    return (ip == p_endpoint.ip) && (port == p_endpoint.port);
  }

  bool operator!=(const endpoint &p_endpoint) const {
    return (ip != p_endpoint.ip) || (port != p_endpoint.port);
  }

  bool operator<(const endpoint &p_endpoint) const {
    if (ip < p_endpoint.ip) {
      return true;
    }
    if (ip > p_endpoint.ip) {
      return false;
    }
    if (port < p_endpoint.port) {
      return true;
    }
    return false;
  }

  std::string ip;
  uint16_t port{0};

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const endpoint &p_endpoint) {
    p_out << '{' << p_endpoint.ip << ',' << p_endpoint.port << '}';
    return p_out;
  }
};

} // namespace tenacitas::lib::async::typ

#endif
