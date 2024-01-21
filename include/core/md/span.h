// Copyright (C) 2023, 2024 by Mark Melton
//

#pragma once
#include <optional>
#include <type_traits>

#include <mdspan/mdspan.hpp>

#include "core/md/iterator.h"
#include "traits.h"

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

inline constexpr auto all = Kokkos::full_extent;
using x = std::pair<int, int>;

namespace detail {
inline auto normalize(int n, std::pair<int, int> extent) {
    auto from = extent.first < 0 ? extent.first + n : extent.first;
    auto to = extent.second < 0 ? extent.second + n : extent.second;
    return std::make_pair(std::min(n, std::max(0, from)),
			  std::min(n, std::max(0, to)));
}

inline auto normalize(size_t n, Kokkos::full_extent_t ext) { return ext; }
}; // namespace detail

template <SpanLike T, class S0>
    requires(std::decay_t<T>::extents_type::rank() == 1)
auto slice(T &&span, S0 s0) {
    return Kokkos::Experimental::submdspan(std::forward<T>(span),
                                           detail::normalize(span.extent(0), s0));
}

template <SpanLike T, class S0, class S1>
    requires(std::decay_t<T>::extents_type::rank() == 2)
auto slice(T &&span, S0 s0, S1 s1) {
    return Kokkos::Experimental::submdspan(std::forward<T>(span),
                                           detail::normalize(span.extent(0), s0),
                                           detail::normalize(span.extent(1), s1));
}

template <SpanLike T, class S0, class S1, class S2>
    requires(std::decay_t<T>::extents_type::rank() == 3)
auto slice(T &&span, S0 s0, S1 s1, S2 s2) {
    return Kokkos::Experimental::submdspan(std::forward<T>(span),
                                           detail::normalize(span.extent(0), s0),
                                           detail::normalize(span.extent(1), s1),
                                           detail::normalize(span.extent(2), s2));
}

}; // namespace core::md
