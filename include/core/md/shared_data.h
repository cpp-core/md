// Copyright (C) 2023, 2024 by Mark Melton
//

#pragma once
#include <memory>

#include "span.h"

namespace core::md {
namespace detail {

template <class ElementType>
struct shared_ownership_handle {
    std::shared_ptr<ElementType[]> base;
    size_t offset;
};

template <class ElementType>
auto make_shared_ownership_handle(size_t n, size_t offset = 0) {
    return shared_ownership_handle<ElementType>{std::make_shared<ElementType[]>(n), offset};
}

template <std::integral... Sizes>
constexpr size_t product_dynamic_extents(Sizes... sizes) {
    std::array<size_t, sizeof...(Sizes)> arr{static_cast<size_t>(sizes)...};
    size_t n{1};
    for (auto elem : arr)
        n *= elem;
    return n;
}

template <class ElementType>
struct shared_accessor {
    using offset_policy = shared_accessor;
    using element_type = ElementType;
    using reference = ElementType &;
    using data_handle_type = shared_ownership_handle<ElementType>;

    constexpr shared_accessor() noexcept = default;

    template <class OtherElementType>
    constexpr shared_accessor(shared_accessor<OtherElementType>) noexcept {};

    constexpr data_handle_type offset(data_handle_type h, size_t i) const noexcept {
        return data_handle_type{h.base, h.offset + i};
    }

    constexpr reference access(data_handle_type h, size_t i) const noexcept {
        return h.base.get()[h.offset + i];
    };
};

}; // namespace detail

template <class ElementType, size_t Rank, std::integral IndexType = int>
using shared_data = Kokkos::mdspan<ElementType,
                                   Kokkos::dextents<int, Rank>,
                                   Kokkos::layout_right,
                                   detail::shared_accessor<ElementType>>;

template <class ElementType, std::integral... Args>
auto make_shared_data(Args... dynamic_extents) {
    return shared_data<ElementType, sizeof...(dynamic_extents)>{
        detail::make_shared_ownership_handle<ElementType>(
            detail::product_dynamic_extents(dynamic_extents...)),
        dynamic_extents...};
}

template <class ElementType, std::integral... Args>
auto make_shared_data(const std::vector<ElementType>& data, Args... dynamic_extents) {
    auto array = shared_data<ElementType, sizeof...(dynamic_extents)>{
        detail::make_shared_ownership_handle<ElementType>(
            detail::product_dynamic_extents(dynamic_extents...)),
        dynamic_extents...};
    std::copy(data.begin(), data.end(), begin(element_wise{array}));
    return array;
}

}; // namespace core::md
