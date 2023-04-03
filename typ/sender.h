#ifndef TENACITAS_LIB_ASYNC_TYP_SENDER_H
#define TENACITAS_LIB_ASYNC_TYP_SENDER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <functional>
#include <map>

#include <tenacitas.lib.async/typ/channel_id.h>
#include <tenacitas.lib.async/typ/message.h>

namespace tenacitas::lib::async::typ {

using sender = std::function<void(const typ::message &)>;

} // namespace tenacitas::lib::async::typ

#endif
