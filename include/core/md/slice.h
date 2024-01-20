// Copyright (C) 2023, 2024 by Mark Melton
//

#pragma once
#include "span.h"
#include "array.h"

namespace core::md {

template <ArrayLike T>
auto slice(T &&array, std::tuple<int, int> &&tup) {
    return slice(array.to_mdspan(), std::move(tup));
}

}; // core::md
