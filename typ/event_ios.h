#ifndef TENACITAS_LIB_ASYNC_TYP_EVENT_IO_H
#define TENACITAS_LIB_ASYNC_TYP_EVENT_IO_H

#include <utility>

#include <tenacitas.lib.async/typ/deserializer.h>
#include <tenacitas.lib.async/typ/channel_id.h>
#include <tenacitas.lib.async/typ/receiver.h>
#include <tenacitas.lib.async/typ/sender.h>
#include <tenacitas.lib.async/typ/serializer.h>

namespace tenacitas::lib::async::typ {

template <typename t_event> struct event_io {
  using serializer = typename typ::serializer<t_event>;

  serializer serialize;
};

} // namespace tenacitas::lib::async::typ

#endif
