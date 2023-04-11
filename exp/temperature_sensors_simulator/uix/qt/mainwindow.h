#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <thread>

#include <QMainWindow>

#include <tenacitas.lib.async/exp/temperature_sensors_simulator/evt/new_temperature.h>
#include <tenacitas.lib.async/alg/dispatcher.h>

using namespace tenacitas::lib;
using namespace temperature_sensors_simulator;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(async::alg::dispatcher::ptr p_dispatcher,
             QWidget *parent = nullptr);
  ~MainWindow();

private:
  void on_new_temperature(evt::new_temperature &&);
  int findRow(typ::sensor_id) const;

private slots:
  void on_btnAddSensor_clicked();
  void on_btnDeleteSensor_clicked();

  void on_btnSetTemperature_clicked();

private:
  async::alg::dispatcher::ptr m_dispatcher;
  Ui::MainWindow *ui;
  std::mutex m_mutex;
};
#endif // MAINWINDOW_H
