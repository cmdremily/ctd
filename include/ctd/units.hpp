#ifndef CTD_UNITS_HPP
#define CTD_UNITS_HPP

#include <cstdint>

#ifdef HAS_STL
#include <ostream>
#endif

#include "ratio.hpp"
#include "numeric.hpp"
#include "units_impl.hpp"

namespace ctd {
    namespace units {
        // The "special" unity unit
        using unity = detail::make_unit_powers<0, 0, 0, 0, 0, 0, 0>;

        // The seven base units
        using ampere = detail::make_unit_powers<1, 0, 0, 0, 0, 0, 0>;
        using kelvin = detail::make_unit_powers<0, 1, 0, 0, 0, 0, 0>;
        using second = detail::make_unit_powers<0, 0, 1, 0, 0, 0, 0>;
        using metre = detail::make_unit_powers<0, 0, 0, 1, 0, 0, 0>;
        using kilogram = detail::make_unit_powers<0, 0, 0, 0, 1, 0, 0>;
        using candela = detail::make_unit_powers<0, 0, 0, 0, 0, 1, 0>;
        using mole = detail::make_unit_powers<0, 0, 0, 0, 0, 0, 1>;

        // Some named units that make the below definitions easier
        using area = detail::unit_powers_add<metre, metre>;
        using volume = detail::unit_powers_add<area, metre>;
        using speed = detail::unit_powers_subtract<metre, second>;
        using acceleration = detail::unit_powers_subtract<speed, second>;

        // The 22 named derived units
        using hertz = detail::unit_powers_subtract<unity, second>;
        using radian = unity;
        using steradian = unity;
        using newton = detail::unit_powers_add<kilogram, acceleration>;
        using pascal = detail::unit_powers_subtract<newton, area>;
        using joule = detail::unit_powers_add<newton, metre>;
        using watt = detail::unit_powers_subtract<joule, second>;
        using coulomb = detail::unit_powers_add<ampere, second>;
        using volt = detail::unit_powers_subtract<joule, coulomb>;
        using farad = detail::unit_powers_subtract<coulomb, volt>;
        using ohm = detail::unit_powers_subtract<volt, ampere>;
        using siemens = detail::unit_powers_subtract<ampere, volt>;
        using weber = detail::unit_powers_subtract<joule, ampere>;
        using tesla = detail::unit_powers_subtract<weber, area>;
        using henry = detail::unit_powers_subtract<ohm, second>;
        using celsius = kelvin;
        using lumen = candela;
        using lux = detail::unit_powers_subtract<candela, area>;
        using becquerel = hertz;
        using gray = detail::unit_powers_subtract<joule, kilogram>;
        using sievert = gray;
        using katal = detail::unit_powers_subtract<mole, second>;
    }

    template <typename ValueType, typename Units, typename Scale>
    class quantity {
    public:
        using units = Units;
        using scale = Scale;
        using value_type = ValueType;

        constexpr quantity() = default;
        constexpr quantity(value_type val) : v(val) {};
        constexpr quantity(const quantity&) = default;
        constexpr quantity(quantity&&) = default;

        template <typename OtherValueType, typename OtherScale, float_round_style rounding = float_round_style::round_toward_zero>
        constexpr quantity(const quantity<OtherValueType, units, OtherScale>& q)
            : v(ratio_convert<scale, OtherScale, ValueType, rounding>(q.count())) {
        }

        constexpr quantity& operator=(const quantity&) = default;

        constexpr quantity operator-() const { return quantity(-count()); }

        constexpr quantity& operator++() {
            ++v;
            return *this;
        }

        constexpr quantity operator++(int) {
            auto copy = *this;
            ++v;
            return copy;
        }

        constexpr quantity& operator--() {
            --v;
            return *this;
        }

        constexpr quantity operator--(int) {
            auto copy = *this;
            --v;
            return copy;
        }

        constexpr value_type count() const { return v; }

    private:
        value_type v;
    };

    //
    // Convenience type aliases for the seven base units
    // 
    template <typename type, typename scale = ratio<1>>
    using current = quantity<type, units::ampere, scale>;

    template <typename type, typename scale = ratio<1>>
    using temperature = quantity<type, units::kelvin, scale>;

    template <typename type, typename scale = ratio<1>>
    using time = quantity<type, units::second, scale>;

    template <typename type, typename scale = ratio<1>>
    using length = quantity<type, units::metre, scale>;

    template <typename type, typename scale = ratio<1>>
    using mass = quantity<type, units::kilogram, scale>;

    template <typename type, typename scale = ratio<1>>
    using luminosity = quantity<type, units::candela, scale>;

    template <typename type, typename scale = ratio<1>>
    using molar_mass = quantity<type, units::candela, scale>;

    template <typename type, typename scalee = ratio<1>>
    using scale = quantity<type, units::unity, scalee>;

    //
    // Convenience type aliases for derived units (WIP)
    //
    template <typename type, typename scale = ratio<1>>
    using frequency = quantity<type, units::hertz, scale>;

    template <typename type, typename scale = ratio<1>>
    using force = quantity<type, units::newton, scale>;

    template <typename type, typename scale = ratio<1>>
    using voltage = quantity<type, units::volt, scale>;

    template <typename type, typename scale = ratio<1>>
    using capacitance = quantity<type, units::farad, scale>;

    template <typename type, typename scale = ratio<1>>
    using resistance = quantity<type, units::ohm, scale>;

    template <typename type, typename scale = ratio<1>>
    using speed = quantity<type, units::speed, scale>;

    //
    // Basic operators
    //
    template <typename val_l, typename val_r, typename units, typename scales_l, typename scales_r>
    constexpr auto operator+(const quantity<val_l, units, scales_l>& lhs, const quantity<val_r, units, scales_r>& rhs) {
        // We have:
        // x * a/b + y * c/d = (xda + ybc)/(bd)
        // let:
        // g_da = gcd(d, a)
        // g_bc = gcd(b, c)
        // g = gcd(da, bc)
        // then:
        // x * a/b + y * c/d = (xda/g + ybc/g) * g/(bd);
        // I.e. the resulting scale is g/(bd).

        constexpr auto a = scales_l::num;
        constexpr auto b = scales_l::den;
        constexpr auto c = scales_r::num;
        constexpr auto d = scales_r::den;

        constexpr auto g_ad = gcd(a, d);
        constexpr auto g_bc = gcd(b, c);

        constexpr auto a_p = a / g_ad;
        constexpr auto b_p = b / g_bc;
        constexpr auto c_p = c / g_bc;
        constexpr auto d_p = d / g_ad;

        // Note: a_p and d_p are now co-prime, same for b_p and c_p. This means that
        // g_p = gcd(a_p*d_p, b_p*c_p) = gcd(a_p,b_p*c_p)*gcd(d_p,b_p*c_p)
        //     = gcd(a_p, b_p) * gcd(a_p, c_p) * gcd(d_p, b_p) * gcd(d_p, c_p)
        //     = / note that a,b are co-prime and thus a_p and b_p must also be, same for c,d /
        //     = gcd(a_p, c_p) * gcd(d_p, b_p)

        constexpr auto scale_left_p = (a_p / gcd(a_p, c_p)) * (d_p / gcd(d_p, b_p)) * g_ad * g_ad;
        constexpr auto scale_right_p = (b_p / gcd(d_p, b_p)) * (c_p / gcd(a_p, c_p)) * g_bc * g_bc;

        constexpr auto g_s = gcd(scale_left_p, scale_right_p);

        constexpr auto scale_left = scale_left_p / g_s;
        constexpr auto scale_right = scale_right_p / g_s;

        // Scale = g_s * gcd(a_p, c_p) * gcd(d_p, b_p) / (b*d)
        //       = g_s * gcd(a_p, c_p) * gcd(d_p, b_p) / (b_p*d_p*g_ad*g_bc)

        using scale = ratio_multiply<ratio_multiply<ratio<gcd(d_p, b_p), max(b_p, d_p)>::type,
            ratio<gcd(a_p, c_p), min(b_p, d_p)>::type>,
            ratio<g_s, g_ad* g_bc>::type>;

        auto ans = lhs.count() * scale_left + rhs.count() * scale_right;
        return quantity<decltype(ans), units, scale>(ans);
    }

    template <typename val_l, typename val_r, typename units, typename scales_l, typename scales_r>
    constexpr auto operator-(const quantity<val_l, units, scales_l>& lhs,
        const quantity<val_r, units, scales_r>& rhs) {
        return lhs + (-rhs);
    }

    template <typename val_l, typename units_l, typename scales_l, typename val_r, typename units_r,
        typename scales_r>
        constexpr auto operator*(const quantity<val_l, units_l, scales_l>& lhs,
            const quantity<val_r, units_r, scales_r>& rhs) {
        return quantity<decltype(lhs.count()* rhs.count()),
            units::detail::unit_powers_add<units_l, units_r>,
            ratio_multiply<scales_l, scales_r>>(lhs.count() * rhs.count());
    }

    template <typename val_l, typename val_r, typename units_r, typename scales_r>
    constexpr auto operator*(val_l lhs, const quantity<val_r, units_r, scales_r>& rhs) {
        auto ans = lhs * rhs.count();
        return quantity<decltype(ans), units_r, scales_r>(ans);
    }

    template <typename val_l, typename units_l, typename scales_l, typename val_r>
    constexpr auto operator*(const quantity<val_l, units_l, scales_l>& lhs, val_r rhs) {
        return rhs * lhs;
    }

    template <typename val_l, typename units_l, typename scales_l, typename val_r, typename units_r,
        typename scales_r>
        constexpr auto operator/(const quantity<val_l, units_l, scales_l>& lhs,
            const quantity<val_r, units_r, scales_r>& rhs) {
        return quantity<decltype(lhs.count() / rhs.count()),
            units::detail::unit_powers_subtract<units_l, units_r>,
            ratio_divide<scales_l, scales_r>>(lhs.count() / rhs.count());
    }

    template <typename val_l, typename val_r, typename units_r, typename scales_r>
    constexpr auto operator/(const val_l lhs, const quantity<val_r, units_r, scales_r>& rhs) {
        using units = units::detail::unit_powers_subtract<units::unity, units_r>;
        return quantity<val_l, units, ratio<1, scales_r::num>>(lhs * scales_r::den / rhs.count());
    }

    template <typename val_l, typename units_l, typename scales_l, typename val_r>
    constexpr auto operator/(const quantity<val_l, units_l, scales_l>& lhs, const val_r rhs) {
        return lhs / quantity<val_r, units::unity, ratio<1>>(rhs);
    }

    template <typename val_l, typename val_r, typename units, typename scales_l, typename scales_r>
    constexpr auto operator==(const quantity<val_l, units, scales_l>& lhs,
        const quantity<val_r, units, scales_r>& rhs) {
        return 0 == (lhs - rhs).count();
    }

    template <typename val_l, typename val_r, typename units, typename scales_l, typename scales_r>
    constexpr auto operator<(const quantity<val_l, units, scales_l>& lhs,
        const quantity<val_r, units, scales_r>& rhs) {
        // We have:
        // x*a/b < y*c/d <=> x*a*d < y*c*b
        // let:
        // g = gcd(a*d, c*b)
        // then:
        // x*(a*d)/g < y*(c*b)/g

        // This is a compile time mult. compiler should warn of overflow
        constexpr auto da = scales_r::den * scales_l::num;
        constexpr auto bc = scales_l::den * scales_r::num;
        constexpr auto g = gcd(da, bc);

        // FIXME: Compute without overflow
        return lhs.count() * (da / g) < rhs.count() * (bc / g);
    }

    template <typename val_l, typename val_r, typename units, typename scales_l, typename scales_r>
    constexpr auto operator>(const quantity<val_l, units, scales_l>& lhs,
        const quantity<val_r, units, scales_r>& rhs) {
        return rhs < lhs;
    }

    template <typename val_l, typename val_r, typename units, typename scales_l, typename scales_r>
    constexpr auto operator<=(const quantity<val_l, units, scales_l>& lhs,
        const quantity<val_r, units, scales_r>& rhs) {
        return !(rhs < lhs);
    }

    template <typename val_l, typename val_r, typename units, typename scales_l, typename scales_r>
    constexpr auto operator>=(const quantity<val_l, units, scales_l>& lhs,
        const quantity<val_r, units, scales_r>& rhs) {
        return !(lhs < rhs);
    }

    template <typename val_l, typename val_r, typename units, typename scales_l, typename scales_r>
    constexpr auto operator!=(const quantity<val_l, units, scales_l>& lhs,
        const quantity<val_r, units, scales_r>& rhs) {
        return !(lhs == rhs);
    }

#ifdef HAS_STL
    template <typename Val, typename Units, typename Scales>
    std::ostream& operator<<(std::ostream& os, const quantity<Val, Units, Scales>& q) {
        os << ctd::ratio_scale<Scales>(static_cast<double>(q.count()));
        if (!is_same<typename units::unity, Units>::value) {
            os << ' ' << Units();
        }
        return os;
    }
#endif


    template <typename ValueType, typename Units, typename Scale, long long x>
    constexpr auto make_unity_valued() {
        return quantity<ValueType, Units, ratio_multiply<Scale, ratio<x>>>(1);
    }

    // Create a new quantity type such that the given quantity is represented with count()==1.
    template <long long x, typename q>
    constexpr auto make_unity_valued(q) {
        return make_unity_valued<typename q::value_type, typename q::units, typename q::scale, x>();
    }


    namespace unit_literals {

#define MAKE_LITERAL(LIT, UNIT, TYPE, SCALE)                   \
  constexpr auto operator"" _##LIT(unsigned long long int x) { \
    using type = quantity<TYPE, UNIT, SCALE>;           \
    return type(static_cast<type::value_type>(x));             \
  }

#define MAKE_LITERAL_PREFIXES(LIT, UNIT, TYPE)                  \
  constexpr auto operator"" _f##LIT(unsigned long long int x) { \
    using type = quantity<TYPE, UNIT, femto>;             \
    return type(static_cast<type::value_type>(x));              \
  }                                                             \
  constexpr auto operator"" _p##LIT(unsigned long long int x) { \
    using type = quantity<TYPE, UNIT, pico>;             \
    return type(static_cast<type::value_type>(x));              \
  }                                                             \
  constexpr auto operator"" _n##LIT(unsigned long long int x) { \
    using type = quantity<TYPE, UNIT, nano>;             \
    return type(static_cast<type::value_type>(x));              \
  }                                                             \
  constexpr auto operator"" _u##LIT(unsigned long long int x) { \
    using type = quantity<TYPE, UNIT, micro>;            \
    return type(static_cast<type::value_type>(x));              \
  }                                                             \
  constexpr auto operator"" _m##LIT(unsigned long long int x) { \
    using type = quantity<TYPE, UNIT, milli>;            \
    return type(static_cast<type::value_type>(x));              \
  }                                                             \
  constexpr auto operator"" _##LIT(unsigned long long int x) {  \
    using type = quantity<TYPE, UNIT, ratio<1>>;         \
    return type(static_cast<type::value_type>(x));              \
  }                                                             \
  constexpr auto operator"" _k##LIT(unsigned long long int x) { \
    using type = quantity<TYPE, UNIT, kilo>;             \
    return type(static_cast<type::value_type>(x));              \
  }                                                             \
  constexpr auto operator"" _M##LIT(unsigned long long int x) { \
    using type = quantity<TYPE, UNIT, mega>;             \
    return type(static_cast<type::value_type>(x));              \
  }                                                             \
  constexpr auto operator"" _G##LIT(unsigned long long int x) { \
    using type = quantity<TYPE, UNIT, giga>;             \
    return type(static_cast<type::value_type>(x));              \
  }

        MAKE_LITERAL(ppm, units::unity, long, micro)
            MAKE_LITERAL(ppt, units::unity, long, milli)

            MAKE_LITERAL(g, units::kilogram, long, milli)
            MAKE_LITERAL(kg, units::kilogram, long, ratio<1>)
            MAKE_LITERAL(ton, units::kilogram, long, kilo)

            MAKE_LITERAL(km_h, units::speed, long, ratio<1>)
            MAKE_LITERAL(m_s2, units::acceleration, long, ratio<1>)

            MAKE_LITERAL_PREFIXES(s, units::second, long)
            MAKE_LITERAL(min, units::second, long, ratio<60L>)
            MAKE_LITERAL(h, units::second, long, ratio<60L * 60L>)
            MAKE_LITERAL(days, units::second, long, ratio<60L * 60L * 24L>)

            MAKE_LITERAL_PREFIXES(K, units::kelvin, long)

            MAKE_LITERAL_PREFIXES(m, units::metre, long)
            MAKE_LITERAL_PREFIXES(N, units::newton, long)
            MAKE_LITERAL_PREFIXES(Pa, units::pascal, long)
            MAKE_LITERAL_PREFIXES(J, units::joule, long)
            MAKE_LITERAL_PREFIXES(W, units::watt, long)

            MAKE_LITERAL_PREFIXES(Hz, units::hertz, long)
            MAKE_LITERAL_PREFIXES(A, units::ampere, long)
            MAKE_LITERAL_PREFIXES(V, units::volt, long)
            MAKE_LITERAL_PREFIXES(Ohm, units::ohm, long)
            MAKE_LITERAL_PREFIXES(F, units::farad, long)
            MAKE_LITERAL_PREFIXES(H, units::henry, long)
            MAKE_LITERAL_PREFIXES(C, units::coulomb, long)

#undef MAKE_LITERAL
#undef MAKE_LITERAL_PREFIXES
    }  // namespace unit_literals
}  // namespace ctd


#endif