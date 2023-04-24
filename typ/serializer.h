#ifndef TENACITAS_LIB_ASYNC_TYP_SERIALIZER_H
#define TENACITAS_LIB_ASYNC_TYP_SERIALIZER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <algorithm>
#include <functional>
#include <map>
#include <optional>
#include <ranges>

#include <tenacitas.lib.async/cpt/concepts.h>
#include <tenacitas.lib.async/typ/channel_id.h>
#include <tenacitas.lib.async/typ/message.h>

namespace tenacitas::lib::async::typ {

template <cpt::event t_event, std::ranges::range t_message>
using serializer = std::function<std::optional<t_message>(const t_event &)>;

} // namespace tenacitas::lib::async::typ

#endif
