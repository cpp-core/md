// Copyright (C) 2023 by Mark Melton
//

#pragma once
#include <iostream>
#include "core/md/concept.h"

std::ostream& operator<<(std::ostream& os, const core::md::Span auto& span) {
    using T = decltype(span);
    using E = typename std::decay_t<T>::extents_type;
    if constexpr (E::rank() == 1) {
	os << "[ ";
	for (auto i = 0; i < span.extent(0); ++i)
	    os << span[i] << " ";
	os << "]";
    } else if constexpr (E::rank() == 2) {
	for (auto i = 0; i < span.extent(0); ++i) {
	    os << "[ ";
	    for (auto j = 0; j < span.extent(1); ++j)
		os << span[i, j] << " ";
	    os << "]" << std::endl;
	}
    } else if constexpr (E::rank() == 3) {
	for (auto i = 0; i < span.extent(0); ++i) {
	    os << "[ ";
	    for (auto j = 0; j < span.extent(1); ++j) {
		os << "[ ";
		for (auto k = 0; k < span.extent(2); ++k)
		    os << span[i, j, k] << " ";
		os << "]" << std::endl;
	    }
	    os << "]" << std::endl;
	}
    }
    return os;
}
