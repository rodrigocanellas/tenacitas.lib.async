
/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <concepts>
#include <iostream>

#include <tenacitas.lib.async/cpt/concepts.h>
#include <tenacitas.lib.async/typ/event_id.h>
#include <tenacitas.lib.log/alg/logger.h>
#include <tenacitas.lib.program/alg/options.h>
#include <tenacitas.lib.test/alg/tester.h>

using namespace tenacitas::lib;

struct a {
  int i{-3};
  friend std::ostream &operator<<(std::ostream &p_out, const a &p_a) {
    p_out << p_a.i;
    return p_out;
  }
  static constexpr uint16_t id{4};
};

struct b {
  static constexpr int id{4};
};

struct c {
  int i{-3};
  friend std::ostream &operator<<(std::ostream &p_out, const c &p_c) {
    p_out << p_c.i;
    return p_out;
  }
  static constexpr int16_t id{4};
};

struct d {
  int i{-3};
  friend std::ostream &operator<<(std::ostream &p_out, const d &p_d) {
    p_out << p_d.i;
    return p_out;
  }
  static constexpr size_t id{4};
};

struct test000 {
  static std::string desc() {
    return "checks the tenacitas::lib::async::cpt::event concept";
  }

  bool operator()(const program::alg::options &) {
    a _a;

    if (a::id != f<a>(_a)) {
      TNCT_LOG_ERR("a::id != f<a>");
      return false;
    }
    TNCT_LOG_TST("a::id == f<a>: ", a::id, " = ", f<a>(_a));
    return true;
  }

private:
  template <async::cpt::event t_event>
  async::typ::event_id f(const t_event &p_event) {

    std::cout << p_event;
    return t_event::id;
  }
};

struct test002 {
  static std::string desc() {
    return "causes a compiler error due to unsatisfied 'printable' concept";
  }

  bool operator()(const program::alg::options &) {

    // UNCOMENT THE LINES BELOW TO SEE A COMPILER ERROR BECAUSE 'b' does not
    // implment << and 'cpt::event_id'
    //    b _b;
    //    f(_b);
    return true;
  }

private:
  template <async::cpt::event t_event> void f(const t_event &p_event) {
    std::cout << p_event << "," << t_event::id;
  }
};

struct test003 {
  static std::string desc() {
    return "checks the tenacitas::lib::async::cpt::event is not satisfied "
           "because 'id' is not 'std::unsigned_integral'";
  }

  bool operator()(const program::alg::options &) {
    // UNCOMENT THE LINES BELOW TO SEE THE COMPILER ERROR
    //    c _c;
    //    TNCT_LOG_TST("a::id == f<a>: ", c::id, " = ", f<c>(_c));
    return true;
  }

private:
  template <async::cpt::event t_event>
  async::typ::event_id f(const t_event &p_event) {

    std::cout << p_event;
    return t_event::id;
  }
};

struct test004 {
  static std::string desc() {
    return "checks the tenacitas::lib::async::cpt::event concept";
  }

  bool operator()(const program::alg::options &) {
    d _d;

    if (d::id != f<d>(_d)) {
      TNCT_LOG_ERR("a::id != f<a>");
      return false;
    }
    TNCT_LOG_TST("d::id == f<d>: ", d::id, " = ", f<d>(_d));
    return true;
  }

private:
  template <async::cpt::event t_event>
  async::typ::event_id f(const t_event &p_event) {

    std::cout << p_event;
    return t_event::id;
  }
};

struct test005 {
  static std::string desc() {
    return "checks the tenacitas::lib::async::cpt::dispathcer";
  }

  bool operator()(const program::alg::options &) {

    struct disp {
      void stop() { TNCT_LOG_TST("stop"); }
    };

    disp _disp;
    f(_disp);
    return true;
  }

private:
  void f(async::cpt::dispatcher auto &p_disp) { p_disp.stop(); }
};

int main(int argc, char **argv) {
  log::alg::set_debug_level();
  test::alg::tester _tester(argc, argv);

  run_test(_tester, test000);
  run_test(_tester, test002);
  run_test(_tester, test004);
  run_test(_tester, test005);
}
