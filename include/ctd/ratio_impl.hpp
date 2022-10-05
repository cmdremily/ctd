/**
* This file provides a compatible implementation of <ratio> for systems where STL isn't present.
*/
#ifndef CTD_RATIO_IMPL_HPP
#define CTD_RATIO_IMPL_HPP

#include "algorithm.hpp"
#include "cmath.hpp"
#include "limits.hpp"
#include "numeric.hpp"
#include "type_traits.hpp"

#include <cstdint>

namespace ctd_impl {

    static_assert(sizeof(intmax_t) == 8, "");

    template <intmax_t NUM, intmax_t DEN = 1>
    struct ratio {
        constexpr static intmax_t num = sign(DEN) * NUM / gcd(NUM, DEN);
        constexpr static intmax_t den = abs(DEN) / gcd(NUM, DEN);

        constexpr static intmax_t value_round = (num + den / 2) / den;

        using type = ratio<num, den>;
    };


    template <typename R1, typename R2>
    using ratio_add = ratio<R1::num* R2::den + R2::num * R1::den, R1::den* R2::den>::type;

    template <typename R1, typename R2>
    using ratio_subtract = ratio<R1::num* R2::den - R2::num * R1::den, R1::den* R2::den>::type;

    template <typename R1, typename R2>
    using ratio_multiply = ratio<R1::num* R2::num, R1::den* R2::den>::type;

    template <typename R1, typename R2>
    using ratio_divide =
        ratio<(R1::num / gcd(R1::num, R2::num))* (R2::den / gcd(R2::den, R1::den)),
        (R1::den / gcd(R2::den, R1::den))* (R2::num / gcd(R1::num, R2::num))>::type;

    template <typename R1>
    using ratio_invert = ratio<R1::den, R1::num>::type;

    template <typename R1, typename R2>
    struct ratio_less : bool_constant < R1::num* R2::den<R2::num* R1::den> {};

    // template <typename R1, typename R2>
    // constexpr bool ratio_less_v = ratio_less<R1, R2>::value;

    template <typename R1, typename R2>
    struct ratio_less_equal : bool_constant < R1::num* R2::den<R2::num* R1::den> {};

    // template <typename R1, typename R2>
    // constexpr bool ratio_less_equal_v = ratio_less<R1, R2>::value;

    template <typename R1, typename R2>
    struct ratio_equal : bool_constant<R1::num* R2::den == R2::num * R1::den> {};

    // template <typename R1, typename R2>
    // constexpr bool ratio_less_equal_v = ratio_less<R1, R2>::value;

    // typedef ratio<1, 1000000000000000000000000ULL> yocto;
    // typedef ratio<1, 1000000000000000000000ULL> zepto;
    typedef ratio<1, 1000000000000000000ULL> atto;
    typedef ratio<1, 1000000000000000ULL> femto;
    typedef ratio<1, 1000000000000ULL> pico;
    typedef ratio<1, 1000000000ULL> nano;
    typedef ratio<1, 1000000ULL> micro;
    typedef ratio<1, 1000ULL> milli;
    typedef ratio<1, 100ULL> centi;
    typedef ratio<1, 10ULL> deci;
    typedef ratio<10ULL, 1> deca;
    typedef ratio<100ULL, 1> hecto;
    typedef ratio<1000ULL, 1> kilo;
    typedef ratio<1000000ULL, 1> mega;
    typedef ratio<1000000000ULL, 1> giga;
    typedef ratio<1000000000000ULL, 1> tera;
    typedef ratio<1000000000000000ULL, 1> peta;
    typedef ratio<1000000000000000000ULL, 1> exa;
    // typedef ratio<1000000000000000000000ULL, 1> zetta;
    // typedef ratio<1000000000000000000000000ULL, 1> yotta;

}  // namespace ctd_impl

#endif