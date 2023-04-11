QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tenacitas.lib.async.tst.dispatcher

include (../../../../../tenacitas.bld/qtcreator/common.pri)

HEADERS = \
            $$BASE_DIR/tenacitas.lib.async/tst/dispatcher/bus.h \
            $$BASE_DIR/tenacitas.lib.async/tst/dispatcher/cfg.h \
            $$BASE_DIR/tenacitas.lib.async/tst/dispatcher/evt.h \
            $$BASE_DIR/tenacitas.lib.async/tst/dispatcher/per.h \
            $$BASE_DIR/tenacitas.lib.async/tst/dispatcher/sto.h \
            $$BASE_DIR/tenacitas.lib.async/tst/dispatcher/typ.h \
            $$BASE_DIR/tenacitas.lib.async/tst/dispatcher/uix.h

SOURCES = $$BASE_DIR/tenacitas.lib.async/tst/dispatcher/main.cpp

LIBS+=-lsqlite3
