#ifndef TENACITAS_LIB_ASYNC_TYP_RECEIVER_H
#define TENACITAS_LIB_ASYNC_TYP_RECEIVER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <functional>
#include <optional>

#include <tenacitas.lib.async/typ/channel_id.h>
#include <tenacitas.lib.async/typ/message.h>

namespace tenacitas::lib::async::typ {

using receiver = std::function<std::optional<typ::message>()>;

} // namespace tenacitas::lib::async::typ

#endif
