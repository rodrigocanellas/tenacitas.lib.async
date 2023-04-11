QT-=core
TEMPLATE=app
CONFIG+=test
TARGET=tenacitas.lib.async.tst.sleeping_loop

include (../../../../../tenacitas.bld/qtcreator/common.pri)

SOURCES = $$BASE_DIR/tenacitas.lib.async/tst/sleeping_loop/main.cpp

