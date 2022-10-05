#ifndef CTD_STL_SWITCH_HPP
#define CTD_STL_SWITCH_HPP

namespace ctd {
#ifdef HAS_STL
    using namespace std;
#else
    using namespace ctd_impl;
#endif // HAS_STL
}

#endif