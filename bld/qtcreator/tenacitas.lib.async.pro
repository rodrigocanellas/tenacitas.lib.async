TEMPLATE=subdirs

SUBDIRS = \
    exp \
    tst

include (../../../tenacitas.bld/qtcreator/common.pri)

HEADERS = \
        $$BASE_DIR/tenacitas.lib.async/alg/dispatcher.h \
        $$BASE_DIR/tenacitas.lib.async/alg/execute.h \
        $$BASE_DIR/tenacitas.lib.async/alg/sleeping_loop.h \
        $$BASE_DIR/tenacitas.lib.async/cpt/concepts.h \
        $$BASE_DIR/tenacitas.lib.async/internal/typ/queue.h \
        $$BASE_DIR/tenacitas.lib.async/typ/priority.h \
        $$BASE_DIR/tenacitas.lib.async/typ/queue_id.h \
        $$BASE_DIR/tenacitas.lib.async/typ/subscriber.h \


DISTFILES += \
    $$BASE_DIR/tenacitas.lib.async/README.md
