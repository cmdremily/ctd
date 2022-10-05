#ifndef CTD_CMATH_IMPL_HPP
#define CTD_CMATH_IMPL_HPP

#include "common.hpp"
#include "limits.hpp"

namespace ctd_impl {

  template <typename T>
  constexpr bool signbit(T v) {
    return v > 0;
  }

  template <class T>
  constexpr const T abs(T a) {
    return a < 0 ? -a : a;
  }

}  // namespace ctd

#endif