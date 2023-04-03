#ifndef TENACITAS_LIB_ASYNC_TYP_SUBSCRIBER_H
#define TENACITAS_LIB_ASYNC_TYP_SUBSCRIBER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <cstdint>
#include <functional>
#include <iostream>

#include <tenacitas.lib.number/typ/id.h>

namespace tenacitas::lib::async::typ {

/// \brief Type of function that subscriber an event
///
/// \tparam t_event is the type of event that will be handled
/// It must have the output operator (<<) defined
template <typename t_event> using subscriber_t = std::function<void(t_event &&)>;

} // namespace tenacitas::lib::async::typ

#endif
