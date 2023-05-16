// Copyright (C) 2023 by Mark Melton
//

#pragma once
#include <array>
#include "concept.h"

namespace core::md {

template<std::integral T, size_t R>
class MultiIndex {
public:
    static constexpr auto Rank = R;
    using IndexType = T;
    using Array = std::array<IndexType, Rank>;

    MultiIndex() { }

    MultiIndex(const Array& extents)
	: extents_(extents) {
    }

    template<std::integral U, std::integral... Us>
    MultiIndex(U size, Us... sizes)
	: extents_(std::array{size, sizes...}) {
    }

    template<Span S>
    MultiIndex(const S& span) {
	for (auto i = 0; i < Rank; ++i)
	    extents_[i] = span.extent(i);
    }

    template<Span S>
    MultiIndex(const S& span, bool)
	: MultiIndex(span) {
	index_[0] = extents_[0];
    }

    bool exhausted() const {
	return index_[0] >= extents_[0];
    }
    
    auto end_index() const {
	Array idx{};
	idx[0] = extents_[0];
	return idx;
    }
    
    const auto& index() const {
	return index_;
    }

    auto& index() {
	return index_;
    }

    const auto& operator[](int i) const {
	return index_[i];
    }

    auto& operator[](int i) {
	return index_[i];
    }

    const auto& extents() const {
	return extents_;
    }

    auto extent(size_t n) const {
	return extents_[n];
    }

    auto operator++() {
	increment();
	return *this;
    }

    auto operator++(int) {
	auto tmp = *this;
	increment();
	return tmp;
    }

    auto advance_to_end() {
	std::fill(index_.begin() + 1, index_.end(), 0);
	index_[0] = extents_[0];
	return *this;
    }

    bool operator==(const MultiIndex& other) const {
	return index_ == other.index_ and extents_ == other.extents_;
    }
    
    auto operator<=>(const MultiIndex& other) const {
	for (auto i = 0; i < Rank; ++i)
	    if (auto cmp = index_[i] <=> other[i]; cmp != 0)
		return cmp;
	return std::strong_ordering::equal;
    }

private:
    template<int D = Rank - 1>
    void increment() {
	++index_[D];
	if constexpr (D > 0) {
	    if (index_[D] >= extents_[D]) {
		index_[D] = 0;
		increment<D-1>();
	    }
	}
    }
    
    Array index_{}, extents_{};
};

template<std::integral T, size_t R>
MultiIndex(std::array<T, R>) -> MultiIndex<T, R>;

template<std::integral T, std::integral... Ts>
MultiIndex(T size, Ts... sizes) -> MultiIndex<T, 1 + sizeof...(Ts)>;

template<Span T>
MultiIndex(const T&) -> MultiIndex<typename T::index_type, T::rank()>;

template<Span T>
MultiIndex(const T&, bool) -> MultiIndex<typename T::index_type, T::rank()>;

}; // core::md
