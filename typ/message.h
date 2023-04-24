#ifndef TENACITAS_LIB_ASYNC_TYP_MESSAGE_H
#define TENACITAS_LIB_ASYNC_TYP_MESSAGE_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <concepts>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

namespace tenacitas::lib::async::typ {

using message = std::vector<std::byte>;

template <std::forward_iterator t_iterator>
std::string msg2str(t_iterator p_begin, t_iterator p_end) {
  std::stringstream _stream;
  _stream << '{';
  for (t_iterator _ite = p_begin; _ite != p_end; ++_ite) {
    _stream << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
            << std::left << static_cast<uint16_t>(*_ite) << ' ';
    _stream << static_cast<char>(static_cast<uint16_t>(*_ite)) << ' ';
  }
  _stream << '}';

  return _stream.str();
}

template <std::forward_iterator t_iterator>
std::string msg2hexstr(t_iterator p_begin, t_iterator p_end) {
  std::stringstream _stream;
  _stream << "{ ";
  for (t_iterator _ite = p_begin; _ite != p_end; ++_ite) {
    _stream << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
            << std::right << static_cast<uint16_t>(*_ite) << ' ';
  }
  _stream << '}';

  return _stream.str();
}

} // namespace tenacitas::lib::async::typ

#endif
