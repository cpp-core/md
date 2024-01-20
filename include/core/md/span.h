// Copyright (C) 2023, 2024 by Mark Melton
//

#pragma once
#include "core/md/iterator.h"
#include "traits.h"
#include <mdspan/mdspan.hpp>
#include <type_traits>

namespace core::md {

template <class T>
concept SpanLike = is_instance_of_template_v<std::decay_t<T>, Kokkos::mdspan>;

template <class ElementType, size_t Rank, std::integral IndexType = int>
using span = Kokkos::mdspan<ElementType, Kokkos::dextents<IndexType, Rank>>;

template <class ElementType, std::integral IndexType = int>
using span1 = span<ElementType, 1, IndexType>;

template <class ElementType, std::integral IndexType = int>
using span2 = span<ElementType, 2, IndexType>;

template <class ElementType, std::integral IndexType = int>
using span3 = span<ElementType, 3, IndexType>;

template <class ElementType, size_t... Sizes>
using fixed_span = Kokkos::mdspan<ElementType, Kokkos::extents<int, Sizes>...>;

template <SpanLike T>
struct element_wise {
    T &span;
};

template <SpanLike T>
element_wise(const T &) -> element_wise<const T>;

template <SpanLike T>
element_wise(T &) -> element_wise<T>;

template <SpanLike T>
auto begin(const element_wise<T> &wrapper) {
    return span_iterator{wrapper.span};
}

template <SpanLike T>
auto end(const element_wise<T> &wrapper) {
    return typename span_iterator<T>::sentinel{};
}

template <SpanLike T, class... Slices>
auto slice(T &&span, Slices... slices) {
    return Kokkos::Experimental::submdspan(std::forward<T>(span),
                                           std::forward<Slices>(slices)...);
}

}; // namespace core::md
