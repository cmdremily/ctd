/*
* This file provides an implementation of <cmath> which is from std:: if HAS_STL is true, and from CTD otherwise.
* This file also provides some extensions to functionality provided by std::ratio.
*/
#ifndef CTD_CMATH_HPP
#define CTD_CMATH_HPP

#include "stl_switch.hpp"

#ifdef HAS_STL
#include <cmath>
#else
#include "cmath_impl.hpp"
#endif

namespace ctd {

    template <typename T>
    constexpr auto sign(T v) {
        return v >= 0 ? 1 : -1;
    }

    template <typename T, round_style rounding = round_style::truncate>
    constexpr T divide(T num, T den) {
        if (numeric_limits<T>::is_integer) {
            if (den < 0) {
                return divide<T, rounding>(-num, -den);
            }

            // TODO: use a computation that doesn't overflow
            if (rounding == round_style::nearest) {
                auto d2 = den / 2;
                return (num + (num < 0 ? -d2 : d2)) / den;
            }
            else if (rounding == round_style::truncate) {
                return num / den;
            }
            else if (rounding == round_style::floor) {
                return (num - ((den + 1) / 2)) / den;
            }
            else {  // ceil
                return (num + den - 1) / den;
            }
        }
        else {
            return num / den;
        }
    }
}

#endif