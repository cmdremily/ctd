/*
* This file provides an implementation of <type_traits>. If HAS_STL is true, that implementation is from std::. 
*/
#ifndef CTD_TYPE_TRAITS_HPP
#define CTD_TYPE_TRAITS_HPP

#include "stl_switch.hpp"

#ifdef HAS_STL
#include <type_traits>
#else
#include "type_traits_impl.hpp"
#endif // HAS_STL

#endif
