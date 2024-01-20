// Copyright (C) 2023, 2024 by Mark Melton
//

#pragma once
#include "span.h"

namespace core::md {

template <SpanLike T, SpanLike U>
    requires(T::extents_type::rank() == U::extents_type::rank())
constexpr bool operator==(const T &a, const U &b) {
    using E = typename T::extents_type;
    constexpr auto rank = E::rank();
    for (auto i = 0; i < rank; ++i)
        if (a.extent(i) != b.extent(i))
            return false;
    auto aiter = begin(element_wise{a});
    auto aend = end(element_wise{a});
    auto biter = begin(element_wise{b});
    auto bend = end(element_wise{b});
    while (aiter != aend and biter != bend) {
	if (*aiter != *biter)
	    return false;
	++aiter;
	++biter;
    }
    return true;
}

}; // namespace core::md
