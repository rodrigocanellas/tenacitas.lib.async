TEMPLATE=subdirs

SUBDIRS = \
    exp \
    tst

include (../../../tenacitas.bld/qtcreator/common.pri)

HEADERS = \
    $$BASE_DIR/tenacitas.lib.async/alg/execute.h \
    $$BASE_DIR/tenacitas.lib.async/alg/sleeping_loop.h \
    $$BASE_DIR/tenacitas.lib.async/alg/dispatcher.h \
    $$BASE_DIR/tenacitas.lib.async/alg/messenger.h \
    $$BASE_DIR/tenacitas.lib.async/evt/stop_messenger.h \
    $$BASE_DIR/tenacitas.lib.async/evt/events.h \
    $$BASE_DIR/tenacitas.lib.async/evt/new_connection.h \
    $$BASE_DIR/tenacitas.lib.async/typ/receiver.h \
    $$BASE_DIR/tenacitas.lib.async/typ/subscriber.h \
    $$BASE_DIR/tenacitas.lib.async/typ/deserializer.h \
    $$BASE_DIR/tenacitas.lib.async/typ/serializer.h \
    $$BASE_DIR/tenacitas.lib.async/typ/codec.h \
    $$BASE_DIR/tenacitas.lib.async/typ/endpoint.h \
    $$BASE_DIR/tenacitas.lib.async/typ/message.h \
    $$BASE_DIR/tenacitas.lib.async/typ/connection_id.h \
    $$BASE_DIR/tenacitas.lib.async/typ/channel_id.h \
    $$BASE_DIR/tenacitas.lib.async/typ/event_id.h \
    $$BASE_DIR/tenacitas.lib.async/typ/priority.h \
    $$BASE_DIR/tenacitas.lib.async/typ/queue_id.h \
    $$BASE_DIR/tenacitas.lib.async/typ/sender.h \
    $$BASE_DIR/tenacitas.lib.async/typ/event_ios.h \
    $$BASE_DIR/tenacitas.lib.async/internal/evt/connected.h \
    $$BASE_DIR/tenacitas.lib.async/internal/alg/in_messenger.h \
    $$BASE_DIR/tenacitas.lib.async/internal/alg/out_messenger.h \
    $$BASE_DIR/tenacitas.lib.async/internal/typ/queue.h \
    $$BASE_DIR/tenacitas.lib.async/internal/typ/connection.h \
    $$BASE_DIR/tenacitas.lib.async/internal/typ/connections.h


DISTFILES += \
    $$BASE_DIR/tenacitas.lib.async/README.md
