/**
* This file provides a compatible implementation of <numeric> for systems where STL isn't present.
*/
#ifndef CTD_NUMERIC_IMPL_HPP
#define CTD_NUMERIC_IMPL_HPP

namespace ctd_impl {

    template <typename M, typename N>
    constexpr auto gcd(M m, N n) {
        m = abs(m);
        n = abs(n);

        if (m == n) {
            return m;
        }
        if (m == 0) return n;
        if (n == 0) return m;

        // Remove common factors of 2
        int shift = 0;
        while (((m | n) & 1) == 0) {
            m >>= 1;
            n >>= 1;
            ++shift;
        }

        do {
            while ((n & 1) == 0) n >>= 1;

            if (m > n) {
                swap(m, n);
            }

            n = n - m;
        } while (n != 0);
        return m << shift;
    }

    template <class T>
    constexpr const T midpoint(T smaller, T bigger) {
        if (smaller > bigger) {
            return avg(bigger, smaller);
        }
        return smaller + (bigger - smaller) / 2;
    }
}

#endif