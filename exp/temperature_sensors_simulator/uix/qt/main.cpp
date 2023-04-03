#include "mainwindow.h"

#include <QApplication>

#include <tenacitas.lib.async/exp/temperature_sensors_simulator/per/sensors.h>
#include <tenacitas.lib.async/alg/dispatcher.h>

int main(int argc, char *argv[]) {
  using namespace tenacitas::lib;
  using namespace temperature_sensors_simulator;

  async::alg::dispatcher::ptr _dispatcher{async::alg::dispatcher::create()};

  per::sensors _sensors{_dispatcher};

  QApplication a(argc, argv);
  MainWindow w(_dispatcher);
  w.show();
  return a.exec();
}
