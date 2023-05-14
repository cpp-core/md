// Copyright (C) 2023 by Mark Melton
//

#pragma once
#include <mdspan/mdarray.hpp>

namespace core::md {

template<class ElementType, size_t Rank, std::integral IndexType = int>
using array = Kokkos::Experimental::mdarray<ElementType, Kokkos::dextents<IndexType, Rank>>;

template<class ElementType, size_t... Sizes>
using fixed_array = Kokkos::Experimental::mdarray<ElementType, Kokkos::extents<int, Sizes...>>;

template<class ElementType, 
	 class Extents,
	 class LayoutPolicy = Kokkos::layout_right,
	 class AccessorPolicy = Kokkos::default_accessor<ElementType>>
using mixed_array = Kokkos::Experimental::mdarray<ElementType,
						  Extents,
						  LayoutPolicy,
						  AccessorPolicy>;

}; // core::md
