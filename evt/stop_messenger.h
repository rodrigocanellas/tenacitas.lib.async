#ifndef TENACITAS_LIB_ASYNC_EVENTS_STOP_MESSENGER_H
#define TENACITAS_LIB_ASYNC_EVENTS_STOP_MESSENGER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <iostream>

namespace tenacitas::lib::async::evt {

struct stop_messenger {

  friend std::ostream &operator<<(std::ostream &p_out, const stop_messenger &) {
    p_out << "stop_messenger";
    return p_out;
  }

  static constexpr uint64_t id{1670716426016};
};

} // namespace tenacitas::lib::async::evt

#endif
