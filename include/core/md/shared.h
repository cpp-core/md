// Copyright (C) 2023, 2024 by Mark Melton
//

#pragma once
#include <memory>

#include <mdspan/mdspan.hpp>

namespace core::md {
namespace detail {

template<class ElementType>
struct shared_ownership_handle {
    std::shared_ptr<ElementType> base;
    size_t offset;
};

template<class ElementType>
auto make_shared_ownership_handle(size_t n, size_t offset = 0) {
    return shared_ownership_handle<ElementType>{std::make_shared<ElementType>(), offset};
}

template<std::integral... Sizes>
constexpr size_t product_dynamic_extents(Sizes... sizes) {
    std::array<size_t, sizeof...(Sizes)> arr{static_cast<size_t>(sizes)...};
    size_t n{1};
    for (auto elem : arr)
	n *= elem;
    return n;
}

template<class ElementType>
struct shared_accessor {
    using offset_policy = shared_accessor;
    using element_type = ElementType;
    using reference = ElementType&;
    using data_handle_type = shared_ownership_handle<ElementType>;
    
    constexpr shared_accessor() noexcept = default;
    
    template<class OtherElementType>
    constexpr shared_accessor(shared_accessor<OtherElementType>) noexcept  {};
    
    constexpr data_handle_type offset(data_handle_type h, size_t i) const noexcept {
	return data_handle_type{h.base, h.offset + i};
    }
    
    constexpr reference access(data_handle_type h, size_t i) const noexcept {
	return h.base.get()[h.offset + i];
    };
};

}; // detail

template<class ElementType,
	 class Extents,
	 class LayoutPolicy = Kokkos::layout_right,
	 class AccessorPolicy = detail::shared_accessor<ElementType>>
class mdshared : public Kokkos::mdspan<ElementType, Extents, LayoutPolicy, AccessorPolicy> {
public:
    using Base = Kokkos::mdspan<ElementType, Extents, LayoutPolicy, AccessorPolicy>;

    template<std::integral... Args>
    mdshared(Args... dynamic_extents)
	: Base(detail::make_shared_ownership_handle<ElementType>
	       (detail::product_dynamic_extents(dynamic_extents...)),
	       dynamic_extents...) {
    }
};

template<class ElementType, size_t Rank, std::integral IndexType = int>
using shared = mdshared<ElementType, Kokkos::dextents<IndexType, Rank>>;

}; // core::md
