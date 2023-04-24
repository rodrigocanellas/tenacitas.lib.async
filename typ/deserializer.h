#ifndef TENACITAS_LIB_ASYNC_TYP_DESERIALIZER_H
#define TENACITAS_LIB_ASYNC_TYP_DESERIALIZER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <algorithm>
#include <concepts>
#include <functional>
#include <map>
#include <optional>
#include <ranges>

#include <tenacitas.lib.async/cpt/concepts.h>
#include <tenacitas.lib.async/typ/message.h>

namespace tenacitas::lib::async::typ {

// template <typename t_event>
// using deserializer =
//     std::function<std::optional<t_event>(const typ::message &)>;

// template <cpt::event t_event>
// using deserializer =
//     std::function<std::optional<t_event>(const typ::message &)>;

template <cpt::event t_event, std::ranges::range t_message>
using deserializer = std::function<std::optional<t_event>(const t_message &)>;

// template <typename t_functor, typename t_event>
// concept func = requires {
//   cpt::event<t_event> &&std::invocable<t_functor, const typ::message &>
//   &&std::
//       same_as<typename std::invoke_result_t<t_functor, const typ::message &>,
//               t_event> &&std::copy_constructible<t_functor>;
// };
} // namespace tenacitas::lib::async::typ

#endif
