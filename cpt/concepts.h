#ifndef TENACITAS_LIB_ASYNC_CPT_CONCEPTS_H
#define TENACITAS_LIB_ASYNC_CPT_CONCEPTS_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <concepts>
#include <cstdint>
#include <functional>
#include <iostream>

namespace tenacitas::lib::async::cpt {

template <typename t>
concept event_id =
    std::unsigned_integral<std::remove_const_t<std::remove_reference_t<t>>>;

template <class t>
concept printable = requires(std::ostream &os, t p_t) {
  os << p_t;
};

template <typename t>
concept event = requires(t p_t) {
  { t::id } -> event_id;
  requires printable<t>;
};

template <event t_event> using subscriber = std::function<void(t_event &&)>;

template <typename t>
concept dispatcher = requires(t p_t) {

  {p_t.stop()};
};

} // namespace tenacitas::lib::async::cpt

#endif
