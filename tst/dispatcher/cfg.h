#ifndef TENACITAS_TST_ASYNC_DISPATCHER_CFG_H
#define TENACITAS_TST_ASYNC_DISPATCHER_CFG_H

#include <list>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include <tenacitas.lib.log/alg/logger.h>
#include <tenacitas.lib.program/alg/options.h>

#include <tenacitas.lib.async/tst/dispatcher/typ.h>

namespace cfg {

namespace {
constexpr char total_generator_option[]{"total-generator"};
constexpr char amount_events_option[]{"amount-events"};
constexpr char amount_events_increment_option[]{"amount-events-increment"};
constexpr char interval_option[]{"interval"};
constexpr char interval_increment_option[]{"interval-increment"};
constexpr char total_publishing_option[]{"total-publishing"};
constexpr char amount_subscribers_option[]{"amount-subscribers"};
constexpr char amount_subscribers_increment_option[]{
    "amount-subscribers-increment"};
constexpr char sleep_option[]{"sleep"};
constexpr char sleep_increment_option[]{"sleep-increment"};
constexpr char sqlite_file[]{"sqlite-file"};

} // namespace

void help(const char *p_pgm_name, std::ostream &p_stream) {
  p_stream
      << "This tests tenacitas::lib::async::alg::dispatcher class. There will "
         "be "
         "generated as many combinations of the parameters provided.\n"

      << "Parameters:\n"
      << '\t' << total_generator_option
      << ": list with numbers of events generators, like '--"
      << total_generator_option << " {4,7}'\n"

      << '\t' << amount_events_option
      << ": Total number of events generated per generator, like '--"
      << amount_events_option << " {400, 30}'\n"

      << '\t' << amount_events_increment_option
      << ": Increment added to each amount of event generator to each "
         "generator. For example, if '--"
      << amount_events_increment_option
      << " 50', from the second generator, the actual amount of events "
         "generated is 50 plus than the previous. \n"

      << '\t' << interval_option
      << ": Amount of time, in milliseconds, that each generator will "
         "take between each event generation, like '--"
      << interval_option << " { 300, 150 }'\n"

      << '\t' << interval_increment_option
      << ": Amount of time, in milliseconds, that will be added to the "
         "interval of each generator. For example, if '--"
      << interval_increment_option
      << " 80' the from the second generator, the interval for event "
         "generation will be 80ms greater than the previous one\n"

      << '\t' << total_publishing_option
      << ": list with numbers of events publishings, like '--"
      << total_publishing_option << " {2, 6}'\n"

      << '\t' << amount_subscribers_option
      << ": Amount of subscribers for each subscriber, like '--"
      << amount_subscribers_option << " {8, 12}'\n"

      << '\t' << amount_subscribers_increment_option
      << ": Amount of subscribers that will be added to a publishing. For "
         "example, "
         "if '--"
      << amount_subscribers_increment_option
      << " 8', then from the second subscriber on, for each publishing the "
         "amount of subscribers will be 8 more than the previous one\n"

      << '\t' << sleep_option
      << ": Amount of time, in milliseconds, that every subscriber will pause, "
         "in "
         "order to simulate an actual work, like '--"
      << sleep_option << " 400'\n"

      << '\t' << sleep_increment_option
      << ": Amount of time, in milliseconds, that will be added to each "
         "subscriber's sleep time. For example, if '--"
      << sleep_increment_option
      << " 100', than from the second subscriber will pause for more 100ms "
         "than "
         "the previous one\n"

      << '\t' << sqlite_file
      << ": SQLite file name where the results will be saved"
      << "\n\n\n"

      << "Example: " << p_pgm_name
      << " --total-generator { 1 3 }  --amount-events { "
         "200 500 } --amount-events-increment 50 --interval { 300 150 } "
         "--interval-increment 80 --total-publishing { 2 6 } "
         "--amount-subscribers { "
         "3 12 } --amount-subscribers-increment 8 --sleep { 400 250 } "
         "--sleep-increment 100 --sqlite-file db.sqlite"
      << '\n';
}

struct options {
  options(int p_argc, char **p_argv) {
    program::alg::options _options;
    _options.parse(p_argc, p_argv,
                   {total_generator_option, amount_events_option,
                    amount_events_increment_option, interval_option,
                    interval_increment_option, total_publishing_option,
                    amount_subscribers_option,
                    amount_subscribers_increment_option, sleep_option,
                    sleep_increment_option, sqlite_file});

    m_total_generators = get_total_generator(_options);

    m_amounts_events = get_amount_events(_options);

    m_amount_events_increment = get_amount_events_increment(_options);

    m_intervals = get_intervals(_options);

    m_interval_increment = get_interval_increment(_options);

    m_total_publishings = get_total_publishings(_options);

    m_amounts_subscribers = get_amount_subscribers(_options);

    m_amount_subscribers_increment = get_amount_subscribers_increment(_options);

    m_sleeps = get_sleeps(_options);

    m_sleep_increment = get_sleep_increment(_options);

    m_sqlite_file = get_sqlite_file(_options);

    {
      for (const typ::time &_sleep : m_sleeps) {
        TNCT_LOG_INF("sleep = ", _sleep.count());
      }
    }
  }

  typ::total_list get_total_generators() const { return m_total_generators; }

  typ::amount_list get_amounts_events() const { return m_amounts_events; }

  typ::amount_increment get_amount_events_increment() const {
    return m_amount_events_increment;
  };

  typ::time_list get_intervals() const { return m_intervals; }

  typ::time_increment get_interval_increment() const {
    return m_interval_increment;
  }

  typ::total_list get_total_publishings() const { return m_total_publishings; }

  typ::amount_list get_amounts_subscribers() const {
    return m_amounts_subscribers;
  }

  typ::amount_increment get_amount_subscribers_increment() const {
    return m_amount_subscribers_increment;
  }

  typ::time_list get_sleeps() const { return m_sleeps; }

  typ::time_increment get_sleep_increment() const { return m_sleep_increment; }

  std::string get_sqlite_file() const { return m_sqlite_file; }

  typ::total_list get_total_generator(const program::alg::options &p_options) {
    try {
      std::vector<typ::total> _total_generators;
      {
        std::optional<std::list<std::string>> _maybe =
            p_options.get_set_param(total_generator_option);
        if (_maybe) {
          std::list<std::string> _list{std::move(_maybe.value())};
          for (const std::string &_value : _list) {
            _total_generators.push_back(std::stoi(_value));
          }
        }
      }
      return _total_generators;
    } catch (std::exception &_ex) {
      TNCT_LOG_ERR("ERROR: ", _ex.what());
      throw _ex;
    }
  }

  typ::amount_list get_amount_events(const program::alg::options &p_options) {
    try {
      typ::amount_list _amount_events;
      {
        std::optional<std::list<std::string>> _maybe =
            p_options.get_set_param(amount_events_option);
        if (_maybe) {
          std::list<std::string> _list{std::move(_maybe.value())};
          for (const std::string &_value : _list) {
            _amount_events.push_back(std::stoi(_value));
          }
        }
      }
      return _amount_events;
    } catch (std::exception &_ex) {
      TNCT_LOG_ERR("ERROR: ", _ex.what());
      throw _ex;
    }
  }

  typ::amount_increment
  get_amount_events_increment(const program::alg::options &p_options) {
    try {
      typ::amount_increment _amount_events_increment = 0;
      std::optional<std::string> _maybe =
          p_options.get_single_param(amount_events_increment_option);
      if (_maybe) {
        _amount_events_increment = std::stoi(std::move(_maybe.value()));
      }
      return _amount_events_increment;
    } catch (std::exception &_ex) {
      TNCT_LOG_ERR("ERROR: ", _ex.what());
      throw _ex;
    }
  }

  typ::time_list get_intervals(const program::alg::options &p_options) {
    try {
      typ::time_list _intervals;
      {
        std::optional<std::list<std::string>> _maybe =
            p_options.get_set_param(interval_option);
        if (_maybe) {
          std::list<std::string> _list{std::move(_maybe.value())};
          for (const std::string &_value : _list) {
            _intervals.push_back(std::chrono::milliseconds(std::stoi(_value)));
          }
        }
      }
      return _intervals;
    } catch (std::exception &_ex) {
      TNCT_LOG_ERR("ERROR: ", _ex.what());
      throw _ex;
    }
  }

  typ::time_increment
  get_interval_increment(const program::alg::options &p_options) {
    try {
      typ::time_increment _interval_increment;
      std::optional<std::string> _maybe =
          p_options.get_single_param(interval_increment_option);
      if (_maybe) {
        _interval_increment =
            std::chrono::milliseconds(std::stoi(std::move(_maybe.value())));
        return _interval_increment;
      }
      return typ::time_increment{0};
    } catch (std::exception &_ex) {
      TNCT_LOG_ERR("ERROR: ", _ex.what());
      throw _ex;
    }
  }

  typ::total_list
  get_total_publishings(const program::alg::options &p_options) {
    try {
      typ::total_list _total_publishings;
      {
        std::optional<std::list<std::string>> _maybe =
            p_options.get_set_param(total_publishing_option);
        if (_maybe) {
          std::list<std::string> _list{std::move(_maybe.value())};
          for (const std::string &_value : _list) {
            _total_publishings.push_back(std::stoi(_value));
          }
        }
      }
      return _total_publishings;
    } catch (std::exception &_ex) {
      TNCT_LOG_ERR("ERROR: ", _ex.what());
      throw _ex;
    }
  }

  typ::amount_list
  get_amount_subscribers(const program::alg::options &p_options) {
    try {
      typ::amount_list _amounts_subscribers;
      {
        std::optional<std::list<std::string>> _maybe =
            p_options.get_set_param(amount_subscribers_option);
        if (_maybe) {
          std::list<std::string> _list{std::move(_maybe.value())};
          for (const std::string &_value : _list) {
            _amounts_subscribers.push_back(std::stoi(_value));
          }
        }
      }
      return _amounts_subscribers;
    } catch (std::exception &_ex) {
      TNCT_LOG_ERR("ERROR: ", _ex.what());
      throw _ex;
    }
  }

  typ::amount_increment
  get_amount_subscribers_increment(const program::alg::options &p_options) {
    try {
      typ::amount_increment _amount_subscribers_increment = 0;
      std::optional<std::string> _maybe =
          p_options.get_single_param(amount_subscribers_increment_option);
      if (_maybe) {
        _amount_subscribers_increment = std::stoi(std::move(_maybe.value()));
      }
      return _amount_subscribers_increment;
    } catch (std::exception &_ex) {
      TNCT_LOG_ERR("ERROR: ", _ex.what());
      throw _ex;
    }
  }

  typ::time_list get_sleeps(const program::alg::options &p_options) {
    try {
      typ::time_list _sleeps;
      {
        std::optional<std::list<std::string>> _maybe =
            p_options.get_set_param(sleep_option);
        if (_maybe) {
          std::list<std::string> _list{std::move(_maybe.value())};
          for (const std::string &_value : _list) {
            _sleeps.push_back(std::chrono::milliseconds(std::stoi(_value)));
          }
        }
      }
      return _sleeps;
    } catch (std::exception &_ex) {
      TNCT_LOG_ERR("ERROR: ", _ex.what());
      throw _ex;
    }
  }

  typ::time_increment
  get_sleep_increment(const program::alg::options &p_options) {
    try {
      typ::time_increment _sleep_increment;
      std::optional<std::string> _maybe =
          p_options.get_single_param(sleep_increment_option);
      if (_maybe) {
        _sleep_increment =
            std::chrono::milliseconds(std::stoi(std::move(_maybe.value())));
        return _sleep_increment;
      }
      return typ::time_increment{0};

    } catch (std::exception &_ex) {
      TNCT_LOG_ERR("ERROR: ", _ex.what());
      throw _ex;
    }
  }

  std::string get_sqlite_file(const program::alg::options &p_options) {
    try {
      std::string _db_file;
      std::optional<std::string> _maybe =
          p_options.get_single_param(sqlite_file);
      if (_maybe) {
        _db_file = std::move(_maybe.value());
        return _db_file;
      }
      return "db.sqlite";

    } catch (std::exception &_ex) {
      TNCT_LOG_ERR("ERROR: ", _ex.what());
      throw _ex;
    }
  }

private:
  typ::total_list m_total_generators;

  typ::amount_list m_amounts_events;

  typ::amount_increment m_amount_events_increment;

  typ::time_list m_intervals;

  typ::time_increment m_interval_increment;

  typ::total_list m_total_publishings;

  typ::amount_list m_amounts_subscribers;

  typ::amount_increment m_amount_subscribers_increment;

  typ::time_list m_sleeps;

  typ::time_increment m_sleep_increment;

  std::string m_sqlite_file;
};

} // namespace cfg

#endif // CFG_H
