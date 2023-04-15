//#ifndef TENACITAS_LIB_IPC_EVENTS_H
//#define TENACITAS_LIB_IPC_EVENTS_H

///// \copyright This file is under GPL 3 license. Please read the \p LICENSE
///file
///// at the root of \p tenacitas directory

///// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

//#include <functional>
//#include <iostream>
//#include <memory>
//#include <tuple>
//#include <type_traits>
//#include <vector>

//#include <tenacitas.lib.async/alg/dispatcher.h>
//#include <tenacitas.lib.ipc/bus/connection.h>
//#include <tenacitas.lib.ipc/typ/types.h>
//#include <tenacitas.lib.number/typ/id.h>

// namespace tenacitas::lib::ipc::evt {

// struct new_connection {
//   new_connection() = default;
//   new_connection(const new_connection &) = default;
//   new_connection(new_connection &&) = default;
//   new_connection &operator=(const new_connection &) = default;
//   new_connection &operator=(new_connection &&) = default;

//  new_connection(const typ::endpoint &p_endpoint,
//                 bus::connection::ptr p_connection)
//      : endpoint(p_endpoint), connection(p_connection) {}

//  friend std::ostream &operator<<(std::ostream &p_out,
//                                  const new_connection &p_connected_to) {
//    p_out << "connected_to " << p_connected_to.endpoint;
//    return p_out;
//  }

//  typ::endpoint endpoint;
//  bus::connection::ptr connection;

//  static constexpr async::typ::event_id id{1669763783652};
//};

// template <typename t_message> struct interest_out {
//   interest_out() = default;
//   interest_out(typ::endpoint p_endpoint) : endpoint(p_endpoint) {}
//   friend std::ostream &operator<<(std::ostream &p_out, interest_out &) {
//     p_out << "interest_out";
//     return p_out;
//   }
//   static constexpr async::typ::event_id id{1669763796250};
//   typ::endpoint endpoint;
// };

// struct listen {
//   listen() = default;
//   listen(const listen &) = default;
//   listen(listen &&) = default;
//   listen &operator=(const listen &) = default;
//   listen &operator=(listen &&) = default;

//  listen(const typ::endpoint &p_endpoint,
//         std::function<void(const typ ::endpoint &)> p_passive_connector);

//  friend std::ostream &operator<<(std::ostream &p_out, const listen &p_listen)
//  {
//    p_out << "listen in " << p_listen.endpoint;
//    return p_out;
//  }

//  static constexpr async::typ::event_id id{1669763811055};
//  typ::endpoint endpoint;
//  std::function<void(const typ ::endpoint &)> passive_connector;
//};

// struct error {
//   error() = default;
//   error(const typ::endpoint &p_endpoint, typ::status p_status,
//         std::string_view p_desc = "")
//       : endpoint(p_endpoint), status(p_status), description(p_desc) {}

//  error(typ::endpoint &&p_endpoint, typ::status p_status,
//        std::string_view p_desc = "")
//      : endpoint(std::forward<typ::endpoint>(p_endpoint)), status(p_status),
//        description(p_desc) {}

//  friend std::ostream &operator<<(std::ostream &p_out, const error &p_error) {
//    p_out << "endpoint " << p_error.endpoint << ", status " << p_error.status;
//    if (!p_error.description.empty()) {
//      p_out << ", description '" << p_error.description << '\'';
//    }
//    return p_out;
//  }

//  typ::endpoint endpoint;
//  typ::status status;
//  std::string description;
//  static constexpr async::typ::event_id id{1669763750111};
//};

// struct stop_server {
//   friend std::ostream &operator<<(std::ostream &p_out, stop_server) {
//     p_out << "stop_server";
//     return p_out;
//   }
//   static constexpr async::typ::event_id id{1669763826041};
// };

// struct restart_server {
//   friend std::ostream &operator<<(std::ostream &p_out, restart_server) {
//     p_out << "restart_server";
//     return p_out;
//   }
//   static constexpr async::typ::event_id id{1669763837898};
// };

// template <typename t_connection> struct incoming {
//   incoming() = default;
//   incoming(std::byte p_read, std::shared_ptr<t_connection> p_connection)
//       : m_read(p_read), m_connection(p_connection) {}
//   friend std::ostream &operator<<(std::ostream &p_out,
//                                   const incoming &p_incoming) {
//     p_out << "incoming from " << p_incoming.m_connection->get_id();
//     return p_out;
//   }
//   std::byte m_read;
//   std::shared_ptr<t_connection> m_connection;
//   static constexpr async::typ::event_id id{1669763849388};
// };

// struct file_transfer {
//   file_transfer() = default;

//  file_transfer(const std::string p_file) : file(p_file) {}

//  friend std::ostream &operator<<(std::ostream &p_out,
//                                  const file_transfer &p_file_transfer) {
//    p_out << "file_transfer: '" << p_file_transfer.file << '\'';
//    return p_out;
//  }
//  std::string file;
//  static constexpr async::typ::event_id id{1669763864840};
//};

//} // namespace tenacitas::lib::ipc::evt

//#endif
