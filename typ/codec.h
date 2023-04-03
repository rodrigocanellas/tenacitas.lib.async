#ifndef TENACITAS_LIB_ASYNC_TYP_CODEC_H
#define TENACITAS_LIB_ASYNC_TYP_CODEC_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <algorithm>
#include <functional>
#include <map>
#include <mutex>
#include <optional>
#include <set>
#include <tuple>
#include <utility>

#include <tenacitas.lib.async/typ/channel_id.h>
#include <tenacitas.lib.async/typ/deserializer.h>
#include <tenacitas.lib.async/typ/serializer.h>

namespace tenacitas::lib::async::typ {

///
/// \brief The codec class
///
/// \tparam t_serializer must implement
/// \code
/// std::optional<tenacitas::lib::async::typ::message> operator()(const
/// t_event&) \endcode
///
/// \tparam t_deserializer must implement
/// \code
/// std::optional<t_event> operator()(const tenacitas::lib::async::typ::message
/// &p_message)
/// \endcode
template <typ::channel_id t_channel_id, typename t_event, typename t_serializer,
          typename t_deserializer>
struct codec {
  using event = t_event;
  static constexpr typ::channel_id channel_id = t_channel_id;
  using serializer = t_serializer;
  using deserializer = t_deserializer;
};

} // namespace tenacitas::lib::async::typ

#endif
