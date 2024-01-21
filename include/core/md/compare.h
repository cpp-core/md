// Copyright (C) 2023, 2024 by Mark Melton
//

#pragma once
#include "span.h"

template <core::md::SpanLike T, core::md::SpanLike U>
    requires(T::extents_type::rank() == U::extents_type::rank())
constexpr bool operator==(const T &a, const U &b) {
    using E = typename T::extents_type;
    constexpr auto rank = E::rank();
    for (auto i = 0; i < rank; ++i)
        if (a.extent(i) != b.extent(i))
            return false;
    auto aiter = begin(core::md::element_wise{a});
    auto aend = end(core::md::element_wise{a});
    auto biter = begin(core::md::element_wise{b});
    auto bend = end(core::md::element_wise{b});
    while (aiter != aend and biter != bend) {
	if (*aiter != *biter)
	    return false;
	++aiter;
	++biter;
    }
    return true;
}
