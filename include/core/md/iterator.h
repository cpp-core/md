// Copyright (C) 2023 by Mark Melton
//

#pragma once
#include "index_iterator.h"

namespace core::md {

template<Span T>
struct span_iterator {
    static constexpr auto Rank = T::rank();
    using index_type = typename T::index_type;
    using Mdx = MultiIndex<index_type, Rank>;
    
    using difference_type = std::ptrdiff_t;
    using element_type = typename T::element_type;
    using reference = element_type&;
    using pointer = element_type*;

    struct sentinel { };
    
    span_iterator()
	: span_(nullptr) {
    }

    span_iterator(T& span)
	: span_(&span)
	, mdx_(span) {
    }

    reference operator*() {
	return (*span_)[mdx_.index()];
    }

    reference operator*() const {
	return (*span_)[mdx_.index()];
    }

    pointer operator->() const {
	return &(*span_)[mdx_.index()];
    }

    span_iterator& operator++() {
	++mdx_;
	return *this;
    }

    span_iterator operator++(int) {
	span_iterator tmp = *this;
	++mdx_;
	return tmp;
    }

    bool operator==(sentinel) const {
	return mdx_[0] >= span_->extent(0);
    }

    bool operator==(const span_iterator& other) const {
	return (span_ == other.span_) and (mdx_ == other.mdx_);
    }

private:
    T *span_;
    Mdx mdx_{};
};

}; // core::md

namespace Kokkos {

template<core::md::Span T>
auto begin(T& span) {
    return core::md::span_iterator(span);
}

template<core::md::Span T>
auto end(T& span) {
    return typename core::md::span_iterator<T>::sentinel{};
}

}; // Kokkos

