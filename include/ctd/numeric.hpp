/*
* This file provides an implementation of <numeric> which is from std:: if HAS_STL is true, and from CTD otherwise.
* This file also provides some extensions to functionality provided by std::ratio.
*/
#ifndef CTD_NUMERIC_HPP
#define CTD_NUMERIC_HPP

#include "stl_switch.hpp"

#ifdef HAS_STL
#include <numeric>
#else
#include "numeric_impl.hpp"
#endif

#endif // CTD_NUMERIC_HPP