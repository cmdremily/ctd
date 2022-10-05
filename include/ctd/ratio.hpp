/*
* This file provides an implementation of <ratio> which is from std:: if HAS_STL is true, and from CTD otherwise.
* This file also provides some extensions to functionality provided by std::ratio.
*/
#ifndef CTD_RATIO_HPP
#define CTD_RATIO_HPP

#include "stl_switch.hpp"

#ifdef HAS_STL
#include <ratio>
#else
#include "ratio_impl.hpp"
#endif

#include "limits.hpp"

namespace ctd {
    // Computes x = y*r where r is a ratio<> object.
    template <typename R, typename T, float_round_style rounding = float_round_style::round_toward_zero>  // todo; enable only for r is ratio
    constexpr auto ratio_scale(T value) {
        if (numeric_limits<T>::is_integer) {
            // TODO: use a computation that doesn't overflow
            if (rounding == float_round_style::round_to_nearest) {
                auto p = value * R::num;
                auto sign = p < 0 ? -1 : 1;
                return (p + sign * (R::den / 2)) / R::den;
            }
            else if (rounding == float_round_style::round_toward_neg_infinity) {
                return (value * R::num - ((R::den + 1) / 2)) / R::den;
            }
            else if (rounding == round_toward_infinity) {
                return (value * R::num + R::den - 1) / R::den;
            }
            else { // round_toward_zero or indeterminate
                return value * R::num / R::den;
            }
        }
        else {
            return value * R::num / R::den;
        }
    }

    // Given a value x and two ratios, r_left and r_right, let: y * r_left = x * r_right
    // this function computes 'y' such that the above expression holds within rounding error.
    template <typename r_left, typename r_right, typename T, float_round_style rounding = float_round_style::round_toward_zero>
    constexpr T ratio_convert(T x) {
        using scale = ratio_divide<r_right, r_left>;
        return ratio_scale<scale, T, rounding>(x);
    }

}  // namespace ctd

#endif