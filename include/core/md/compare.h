// Copyright (C) 2023 by Mark Melton
//

#pragma once
#include "concept.h"

template<core::md::Span T, core::md::Span U>
requires (T::extents_type::rank() == U::extents_type::rank())
constexpr bool operator==(const T& a, const U& b) {
    using E = typename T::extents_type;
    constexpr auto rank = E::rank();
    for (auto i = 0; i < rank; ++i)
	if (a.extent(i) != b.extent(i))
	    return false;
    auto aiter = begin(a);
    auto aend = end(a);
    auto biter = begin(b);
    auto bend = end(b);
    while (aiter != aend and biter != bend) {
	if (*aiter != *biter)
	    return false;
	++aiter;
	++biter;
    }
    return true;
}
