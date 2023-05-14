// Copyright (C) 2023 by Mark Melton
//

#pragma once
#include <concepts>
#include <type_traits>

namespace core::md {

template<class T>
concept Span = requires (T a, size_t n) {
    typename T::element_type;
    { a.extent(n) } -> std::integral;
    { a.rank() } -> std::integral;
};

}; // core::md
