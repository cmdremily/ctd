#ifndef CTD_UNITS_IMPL_HPP
#define CTD_UNITS_IMPL_HPP

#ifdef HAS_STL
#include <ostream>
#endif

#include "ratio.hpp"

namespace ctd {
    namespace units {
        namespace detail {
            template <typename A, typename K, typename s, typename m, typename kg, typename cd, typename mol>
            struct unit_powers {
                using ampere = A;
                using kelvin = K;
                using second = s;
                using metre = m;
                using kilogram = kg;
                using candela = cd;
                using mole = mol;

                explicit unit_powers() = default;
            };

            template<int A, int K, int s, int m, int kg, int cd, int mol>
            using make_unit_powers = unit_powers<ratio<A>, ratio<K>, ratio<s>, ratio<m>, ratio<kg>, ratio<cd>, ratio<mol>>;

            template <typename lhs, typename rhs>
            using unit_powers_add = unit_powers<
                ratio_add<typename lhs::ampere, typename rhs::ampere>,
                ratio_add<typename lhs::kelvin, typename rhs::kelvin>,
                ratio_add<typename lhs::second, typename rhs::second>,
                ratio_add<typename lhs::metre, typename rhs::metre>,
                ratio_add<typename lhs::kilogram, typename rhs::kilogram>,
                ratio_add<typename lhs::candela, typename rhs::candela>,
                ratio_add<typename lhs::mole, typename rhs::mole>>;

            template <typename lhs, typename rhs>
            using unit_powers_subtract = unit_powers<
                ratio_subtract<typename lhs::ampere, typename rhs::ampere>,
                ratio_subtract<typename lhs::kelvin, typename rhs::kelvin>,
                ratio_subtract<typename lhs::second, typename rhs::second>,
                ratio_subtract<typename lhs::metre, typename rhs::metre>,
                ratio_subtract<typename lhs::kilogram, typename rhs::kilogram>,
                ratio_subtract<typename lhs::candela, typename rhs::candela>,
                ratio_subtract<typename lhs::mole, typename rhs::mole>>;

#ifdef HAS_STL
            template <typename A, typename K, typename s, typename m, typename kg, typename cd, typename mol>
            std::ostream& operator<<(std::ostream& os, unit_powers<A, K, s, m, kg, cd, mol>) {
#define STRINGIFY2(X) #X
#define STRINGIFY(X) STRINGIFY2(X)
                bool first = true;
#define PRINT_SYMBOL_POS(SYMBOL, CMP)                           \
  do {                                                          \
    if (SYMBOL::num CMP) {                                      \
      if (!first) {                                             \
        os << "*";                                              \
      }                                                         \
      os << STRINGIFY(SYMBOL);                                  \
      first = false;                                            \
      if (SYMBOL::den == 1) {                                   \
        if (SYMBOL::num != 1) {                                 \
          os << "^" << SYMBOL::num;                             \
        }                                                       \
      } else {                                                  \
        os << "^(" << SYMBOL::num << "/" << SYMBOL::den << ")"; \
      }                                                         \
    }                                                           \
  } while (0)

                PRINT_SYMBOL_POS(A, > 0);
                PRINT_SYMBOL_POS(K, > 0);
                PRINT_SYMBOL_POS(s, > 0);
                PRINT_SYMBOL_POS(m, > 0);
                PRINT_SYMBOL_POS(kg, > 0);
                PRINT_SYMBOL_POS(cd, > 0);
                PRINT_SYMBOL_POS(mol, > 0);

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
