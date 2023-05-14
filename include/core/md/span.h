// Copyright (C) 2023 by Mark Melton
//

#pragma once
#include <mdspan/mdspan.hpp>

namespace core::md {

template<class ElementType, size_t Rank, std::integral IndexType = int>
using span = Kokkos::mdspan<ElementType, Kokkos::dextents<IndexType, Rank>>;

template<class ElementType, size_t... Sizes>
using fixed_span = Kokkos::mdspan<ElementType, Kokkos::extents<int, Sizes>...>;

template<class ElementType,
	 class Extents,
	 class LayoutPolicy = Kokkos::layout_right,
	 class AccessorPolicy = Kokkos::default_accessor<ElementType>>
using mixed_span = Kokkos::mdspan<ElementType, Extents, LayoutPolicy, AccessorPolicy>;

}; // core::md
