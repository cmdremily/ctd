#ifndef CTD_UNITS_IMPL_HPP
#define CTD_UNITS_IMPL_HPP

#ifdef HAS_STL
#include <ostream>
#endif

#include "ratio.hpp"

namespace ctd {
    namespace units {
        namespace detail {
            template <int A, int K, int s, int m, int kg, int cd, int mol>
            struct unit_powers {
                constexpr static int ampere = A;
                constexpr static int kelvin = K;
                constexpr static int second = s;
                constexpr static int metre = m;
                constexpr static int kilogram = kg;
                constexpr static int candela = cd;
                constexpr static int mole = mol;

                explicit unit_powers() = default;
            };

            template<int A, int K, int s, int m, int kg, int cd, int mol>
            using make_unit_powers = unit_powers<A, K, s, m, kg, cd, mol>;

            template <typename lhs, typename rhs>
            using unit_powers_add = unit_powers<
                lhs::ampere + rhs::ampere,
                lhs::kelvin + rhs::kelvin,
                lhs::second + rhs::second,
                lhs::metre + rhs::metre,
                lhs::kilogram + rhs::kilogram,
                lhs::candela + rhs::candela,
                lhs::mole + rhs::mole>;

            template <typename lhs, typename rhs>
            using unit_powers_subtract = unit_powers<
                lhs::ampere - rhs::ampere,
                lhs::kelvin - rhs::kelvin,
                lhs::second - rhs::second,
                lhs::metre - rhs::metre,
                lhs::kilogram - rhs::kilogram,
                lhs::candela - rhs::candela,
                lhs::mole - rhs::mole>;

#ifdef HAS_STL
            template <int A, int K, int s, int m, int kg, int cd, int mol>
            std::ostream& operator<<(std::ostream& os, const unit_powers<A, K, s, m, kg, cd, mol>& o) {
#define STRINGIFY2(X) #X
#define STRINGIFY(X) STRINGIFY2(X)
                bool first = true;
#define PRINT_SYMBOL_POS(SYMBOL, CMP)                           \
    if (SYMBOL CMP) {                                           \
      if (!first) {                                             \
        os << "*";                                              \
      }                                                         \
      os << STRINGIFY(SYMBOL);                                  \
      first = false;                                            \
      if (SYMBOL != 1) {                                        \
        os << "^" << abs(SYMBOL);                               \
      }                                                         \
    }                                                           \

                PRINT_SYMBOL_POS(A, > 0);
                PRINT_SYMBOL_POS(K, > 0);
                PRINT_SYMBOL_POS(s, > 0);
                PRINT_SYMBOL_POS(m, > 0);
                PRINT_SYMBOL_POS(kg, > 0);
                PRINT_SYMBOL_POS(cd, > 0);
                PRINT_SYMBOL_POS(mol, > 0);
                os << '/';
                first = true;
                PRINT_SYMBOL_POS(A, < 0);
                PRINT_SYMBOL_POS(K, < 0);
                PRINT_SYMBOL_POS(s, < 0);
                PRINT_SYMBOL_POS(m, < 0);
                PRINT_SYMBOL_POS(kg, < 0);
                PRINT_SYMBOL_POS(cd, < 0);
                PRINT_SYMBOL_POS(mol, < 0);

#undef PRINT_SYMBOL
#undef STRINGIFY2
#undef STRINGIFY
                return os;
            }
#endif
        }  // namespace detail

    }
}

#endif CTD_UNITS_IMPL_HPP
