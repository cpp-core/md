// Copyright (C) 2023 by Mark Melton
//

#pragma once
#include "multi_index.h"

namespace core::md {

template<class T, size_t R>
struct index_iterator {
    static constexpr auto Rank = R;
    using Array = std::array<T, Rank>;
    using Mdx = MultiIndex<T, Rank>;
    
    using difference_type = std::ptrdiff_t;
    using element_type = Array;
    using reference = element_type&;
    using pointer = element_type*;

    index_iterator() { }

    template<Span S>
    index_iterator(const S& span)
	: mdx_(span) {
    }

    template<Span S>
    index_iterator(const S& span, bool)
	: mdx_(span, true) {
    }

    index_iterator(const Array& extents)
	: mdx_(extents) {
    }

    index_iterator(const Array& index, const Array& extents)
	: mdx_(index, extents) {
    }

    reference operator*() {
	return mdx_.index();
    }

    reference operator*() const {
	return mdx_.index();
    }

    pointer operator->() const {
	return &(mdx_.index());
    }

    auto& operator++() {
	++mdx_;
	return *this;
    }

    auto operator++(int) {
	index_iterator tmp = *this;
	++mdx_;
	return tmp;
    }

    auto operator<=>(const index_iterator& other) const = default;

private:
    Mdx mdx_;
};

template<Span T>
index_iterator(const T&) -> index_iterator<typename T::index_type, T::rank()>;

template<Span T>
index_iterator(const T&, bool) -> index_iterator<typename T::index_type, T::rank()>;

}; // core::md

namespace Kokkos {

template<core::md::Span T>
auto begin_index(T& span) {
    return core::md::index_iterator(span);
}
    
template<core::md::Span T>
auto end_index(T& span) {
    return core::md::index_iterator(span, true);
}

}; // Kokkos
