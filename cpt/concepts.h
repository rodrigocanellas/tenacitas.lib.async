#ifndef TENACITAS_LIB_ASYNC_CPT_CONCEPTS_H
#define TENACITAS_LIB_ASYNC_CPT_CONCEPTS_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas.lib.async directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <concepts>
#include <cstdint>
#include <functional>
#include <iostream>

#include <tenacitas.lib.async/typ/event_id.h>
#include <tenacitas.lib.concepts/cpt/printable.h>

namespace tenacitas::lib::async::cpt {

template <typename t>
concept event = requires(t p_t) {
  { t::id } -> std::same_as<const typ::event_id &>;
  concepts::cpt::printable<t>;
  std::default_initializable<t>;
};

// template <typename t>
// concept message_iterator = requires {
//   std::forward_iterator<t>;

//};

// template <typename t>
// concept message = requires {
//   std::copyable<t>;
//   std::movable<t>;
//   typename t::const_iterator;

//};

template <typename t>
concept dispatcher = requires(t p_t) {

  {p_t.stop()};
};

} // namespace tenacitas::lib::async::cpt

#endif
