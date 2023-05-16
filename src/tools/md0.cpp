// Copyright (C) 2023 by Mark Melton
//

#include <algorithm>
#include <compare>
#include <iostream>
#include <ranges>
#include "core/md/array.h"
#include "core/md/span.h"
#include "core/md/shared.h"
#include "core/md/ostream.h"
#include "core/md/concept.h"

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

    template<Span S>
    MultiIndex(const S& span) {
	for (auto i = 0; i < Rank; ++i)
	    extents_[i] = span.extent(i);
    }

    template<Span S>
    MultiIndex(const S& span, bool)
	: MultiIndex(span) {
	index_[0] = extents_[0] + 1;
    }

    bool exhausted() const {
	return index_[0] >= extents_[0];
    }
    
    auto end_index() const {
	Array idx{};
	idx[0] = extents_[0] + 1;
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

    bool operator==(const MultiIndex& other) const {
	return index_ == other.index_;
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

static_assert(std::forward_iterator<index_iterator<int,2>>);
static_assert(std::forward_iterator<index_iterator<int,2>>);

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

static_assert(std::forward_iterator<span_iterator<array<int,2>>>);
static_assert(std::forward_iterator<span_iterator<span<int,2>>>);

}; // core::md

using std::cout, std::endl;

namespace Kokkos {

template<core::md::Span T>
auto begin_index(T& span) {
    return core::md::index_iterator(span);
}
    
template<core::md::Span T>
auto end_index(T& span) {
    return core::md::index_iterator(span, true);
}

template<core::md::Span T>
auto begin(T& span) {
    return core::md::span_iterator(span);
}

template<core::md::Span T>
auto end(T& span) {
    return typename core::md::span_iterator<T>::sentinel{};
}

}; // Kokkos


template<core::md::Span T, core::md::Span U>
requires (T::extents_type::rank() == U::extents_type::rank())
constexpr bool operator==(const T& a, const U& b) {
    using E = typename T::extents_type;
    constexpr auto rank = E::rank();
    for (auto i = 0; i < rank; ++i)
	if (a.extent(i) != b.extent(i))
	    return false;
    auto aiter = begin(a);
    auto aend = end(a);
    auto biter = begin(b);
    auto bend = end(b);
    while (aiter != aend and biter != bend) {
	if (*aiter != *biter)
	    return false;
	++aiter;
	++biter;
    }
    return true;
}

int main(int argc, const char *argv[]) {
    core::md::array<int, 2> x(6, 10);
    core::md::array<int, 2> a(6, 10);
    auto xs = x.to_mdspan();
    auto as = a.to_mdspan();

    for (auto i : std::ranges::iota_view{1, 10})
	cout << i << " ";
    cout << endl;

    std::ranges::fill(begin(xs), end(xs), 42);
    std::ranges::fill(begin(as), end(as), 42);
    assert(a.to_mdspan() == x.to_mdspan());
    
    core::md::fixed_array<int, 3, 3> y{};
    core::md::shared<int, 2> z(7, 8);

    std::ranges::generate(begin(xs), end(xs), [n=0]() mutable { return n++; });
    cout << "generate: " << endl << xs << endl;

    std::ranges::for_each(begin_index(xs), end_index(xs), [&](const auto& index) {
	xs[index[0], index[1]] = index[0] * xs.extent(1) + index[1];
    });
    cout << "for_each: " << endl << xs << endl;

    core::md::fixed_array<int, 3, 3, 3> w{};
    auto wx = w.to_mdspan();
    std::ranges::generate(begin(wx), end(wx), [n=0]() mutable { return n++; });
    cout << w << endl;

    cout << x.size() << endl;
    cout << a.size() << endl;
    cout << y.size() << endl;
    cout << z.size() << endl;
    cout << w.size() << endl;

    for (auto elem : wx)
	cout << elem << " ";
    
    return 0;
}
