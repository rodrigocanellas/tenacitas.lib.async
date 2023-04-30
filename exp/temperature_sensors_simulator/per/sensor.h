#ifndef TEMPERATURE_SENSORS_SIMULATOR__PER__SENSOR_H
#define TEMPERATURE_SENSORS_SIMULATOR__PER__SENSOR_H

#include <thread>

#include <tenacitas.lib.async/alg/dispatcher.h>
#include <tenacitas.lib.async/alg/sleeping_loop.h>
#include <tenacitas.lib.async/exp/temperature_sensors_simulator/evt/new_temperature.h>
#include <tenacitas.lib.async/exp/temperature_sensors_simulator/typ/sensor_id.h>
#include <tenacitas.lib.async/exp/temperature_sensors_simulator/typ/temperature.h>
#include <tenacitas.lib.calendar/cpt/chrono_convertible.h>

using namespace tenacitas::lib;

namespace temperature_sensors_simulator::per {

struct sensor {
  sensor() = delete;
  sensor(const sensor &) = delete;
  sensor(sensor &) = delete;
  sensor &operator=(const sensor &) = delete;
  sensor &operator=(sensor &) = delete;

  sensor(async::alg::dispatcher::ptr p_dispatcher,
         calendar::cpt::convertible_to_ns auto p_interval,
         typ::sensor_id p_sensor_id, typ::temperature p_initial,
         typ::temperature p_increment)
      : m_dispatcher(p_dispatcher), m_sensor_id(p_sensor_id),
        m_current(p_initial), m_increment(p_increment),
        m_sleeping_loop(
            [this]() {
              {
                std::lock_guard<std::mutex> _lock(m_mutex);
                m_current += m_increment;
              }
              m_dispatcher->publish<evt::new_temperature>(m_sensor_id,
                                                          m_current);
            },
            p_interval) {}

  ~sensor() = default;

  void start() { m_sleeping_loop.start(); }

  void stop() { m_sleeping_loop.stop(); }

  bool operator<(const sensor &p_sensor) const {
    return m_sensor_id < p_sensor.m_sensor_id;
  }

  bool operator>(const sensor &p_sensor) const {
    return m_sensor_id > p_sensor.m_sensor_id;
  }

  bool operator==(const sensor &p_sensor) const {
    return m_sensor_id == p_sensor.m_sensor_id;
  }

  bool operator!=(const sensor &p_sensor) const {
    return m_sensor_id != p_sensor.m_sensor_id;
  }

  void reset_temperature(typ::temperature p_temperature) {
    std::lock_guard<std::mutex> _lock(m_mutex);
    m_current = p_temperature;
  }

  typ::sensor_id get_id() const { return m_sensor_id; }

private:
  async::alg::dispatcher::ptr m_dispatcher;
  typ::sensor_id m_sensor_id;
  typ::temperature m_current;
  typ::temperature m_increment;
  async::alg::sleeping_loop m_sleeping_loop;
  std::mutex m_mutex;
};

} // namespace temperature_sensors_simulator::per

#endif // SENSOR_H
