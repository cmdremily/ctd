/*
* This file provides an implementation of <limits>. If HAS_STL is true, that implementation is from std::. 
*/
#ifndef CTD_LIMITS_HPP
#define CTD_LIMITS_HPP

#include "stl_switch.hpp"

#ifdef HAS_STL
#include <limits>
#else
#include "limits_impl.hpp"
#endif // HAS_STL

#endif
