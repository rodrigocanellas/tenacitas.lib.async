QT+=widgets
TEMPLATE=app
CONFIG+=example
TARGET=tenacitas.lib.async.exp.temperature_sensors_simulator

include (../../../../../tenacitas.bld/qtcreator/common.pri)

SOURCES = $$BASE_DIR/tenacitas.lib.async/exp/executer/main.cpp

HEADERS = \
    $$BASE_DIR/tenacitas.lib.async/exp/temperature_sensors_simulator/typ/sensor_id.h \
    $$BASE_DIR/tenacitas.lib.async/exp/temperature_sensors_simulator/typ/temperature.h \
    $$BASE_DIR/tenacitas.lib.async/exp/temperature_sensors_simulator/evt/add_sensor.h \
    $$BASE_DIR/tenacitas.lib.async/exp/temperature_sensors_simulator/evt/new_temperature.h \
    $$BASE_DIR/tenacitas.lib.async/exp/temperature_sensors_simulator/evt/remove_sensor.h \
    $$BASE_DIR/tenacitas.lib.async/exp/temperature_sensors_simulator/evt/set_temperature.h \
    $$BASE_DIR/tenacitas.lib.async/exp/temperature_sensors_simulator/per/sensor.h \
    $$BASE_DIR/tenacitas.lib.async/exp/temperature_sensors_simulator/per/sensors.h \
    $$BASE_DIR/tenacitas.lib.async/exp/temperature_sensors_simulator/uix/qt/mainwindow.h \

SOURCES = \
    $$BASE_DIR/tenacitas.lib.async/exp/temperature_sensors_simulator/uix/qt/mainwindow.cpp \
    $$BASE_DIR/tenacitas.lib.async/exp/temperature_sensors_simulator/uix/qt/main.cpp \

FORMS = \
    $$BASE_DIR/tenacitas.lib.async/exp/temperature_sensors_simulator/uix/qt/mainwindow.ui

