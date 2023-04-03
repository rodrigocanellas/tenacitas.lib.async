#ifndef TENACITAS_LIB_ASYNC_INTERNAL_TYP_CONNECTIONS_H
#define TENACITAS_LIB_ASYNC_INTERNAL_TYP_CONNECTIONS_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <map>

#include <tenacitas.lib.async/internal/typ/connection.h>
#include <tenacitas.lib.async/typ/channel_id.h>

namespace tenacitas::lib::async::internal::typ {
using connections = std::map<async::typ::channel_id, typ::connection::ptr>;

} // namespace tenacitas::lib::async::internal::typ

#endif
