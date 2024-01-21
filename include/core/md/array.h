// Copyright (C) 2023, 2024 by Mark Melton
//

#pragma once
#include <vector>

#include <mdspan/mdarray.hpp>

#include "traits.h"
#include "span.h"

namespace core::md {

template <class T>
concept ArrayLike = is_instance_of_template_v<std::decay_t<T>, Kokkos::Experimental::mdarray>;

template <class ElementType, size_t Rank, std::integral IndexType = int>
using array = Kokkos::Experimental::mdarray<ElementType,
                                            Kokkos::dextents<IndexType, Rank>>;

template <class ElementType, std::integral IndexType = int>
using array1 = array<ElementType, 1, IndexType>;

template <class ElementType, std::integral IndexType = int>
using array2 = array<ElementType, 2, IndexType>;

template <class ElementType, std::integral IndexType = int>
using array3 = array<ElementType, 3, IndexType>;

template <class ElementType, size_t... Sizes>
using fixed_array =
    Kokkos::Experimental::mdarray<ElementType, Kokkos::extents<int, Sizes...>>;

template <ArrayLike T, class... Slicers>
auto slice(T &&array, Slicers... slicers) {
    return slice(array.to_mdspan(), std::forward<Slicers>(slicers)...);
}

template <class T>
auto array_from_vector(std::vector<T> &&vec) {
    return array1<T>{Kokkos::dextents<int, 1>{vec.size()}, std::move(vec)};
}

}; // core::md
