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

#include <tenacitas.lib.concepts/cpt/chrono_convertible.h>
#include <tenacitas.lib.concepts/cpt/id.h>
#include <tenacitas.lib.concepts/cpt/printable.h>

namespace tenacitas::lib::async::cpt {

template <typename t>
concept queue_id = concepts::cpt::id<t>;

template <typename t>
concept event = requires(t p_t) {
  { t::id } -> tenacitas::lib::concepts::cpt::id;
  requires tenacitas::lib::concepts::cpt::printable<t>;
};

template <typename t>
concept dispatcher = requires(t p_t) {

  {p_t.stop()};
};

template <typename t_from>
concept convertible_to_ms =
    tenacitas::lib::concepts::cpt::chrono_convertible<std::chrono::milliseconds,
                                                      t_from>;

template <typename t_from>
concept convertible_to_ns =
    tenacitas::lib::concepts::cpt::chrono_convertible<std::chrono::nanoseconds,
                                                      t_from>;

} // namespace tenacitas::lib::async::cpt

#endif
