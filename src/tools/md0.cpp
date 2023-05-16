// Copyright (C) 2023 by Mark Melton
//

#include <algorithm>
#include <iostream>
#include <ranges>
#include "core/md/array.h"
#include "core/md/span.h"
#include "core/md/shared.h"
#include "core/md/ostream.h"
#include "core/md/concept.h"

namespace core::md {

template<Span T>
struct span_iterator {
    using difference_type = std::ptrdiff_t;
    using element_type = typename T::element_type;
    using reference = element_type&;
    using pointer = element_type*;

    using extents_type = typename T::extents_type;
    using index_type = typename T::index_type;
    static constexpr auto Rank = T::rank();
    
    struct sentinel { };
    
    span_iterator()
	: span_(nullptr) {
    }

    span_iterator(T& span)
	: span_(&span) {
    }

    span_iterator(const span_iterator&) = default;
    span_iterator(span_iterator&&) = default;
    span_iterator& operator=(const span_iterator&) { return *this; };
    span_iterator& operator=(span_iterator&&) { return *this; };

    reference operator*() {
	return (*span_)[index_];
    }

    reference operator*() const {
	return (*span_)[index_];
    }

    pointer operator->() const {
	return &(*span_)[index_];
    }

    span_iterator& operator++() {
	increment();
	return *this;
    }

    span_iterator operator++(int) {
	span_iterator tmp = *this;
	++(*this);
	increment();
	return tmp;
    }

    bool operator==(sentinel) const {
	return index_[0] >= span_->extent(0);
    }

    bool operator==(const span_iterator& other) const {
	return (span_ == other.span_) and (index_ == other.index_);
    }

private:
    template<int D = Rank - 1>
    void increment() {
	++index_[D];
	if constexpr (D > 0) {
	    if (index_[D] >= span_->extent(D)) {
		index_[D] = 0;
		increment<D-1>();
	    }
	}
    }

    T *span_;
    std::array<index_type, Rank> index_{};
};

static_assert(std::forward_iterator<span_iterator<array<int,2>>>);
static_assert(std::forward_iterator<span_iterator<span<int,2>>>);

template<class ForwardIter1, class ForwardIter2, class Value>
void fill(ForwardIter1 begin, ForwardIter2 end, Value value) {
    while (begin != end) {
	*begin = value;
	++begin;
    }
}

}; // core::md

using std::cout, std::endl;

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

    auto r = std::ranges::iota_view{1, 10};
    for (auto i : r)
	cout << i << " ";
    cout << endl;
    std::ranges::fill(begin(xs), end(xs), 42);
    std::ranges::fill(begin(as), end(as), 42);
    assert(a.to_mdspan() == x.to_mdspan());
    
    core::md::fixed_array<int, 3, 3> y{};
    core::md::shared<int, 2> z(7, 8);

    for (auto i = 0; i < x.extent(0); ++i)
	for (auto j = 0; j < x.extent(1); ++j)
	    x[i, j] = i * x.extent(1) + j;
    
    for (auto i = 0; i < x.extent(0); ++i) {
	for (auto j = 0; j < x.extent(1); ++j)
	    cout << x[i, j] << " ";
	cout << endl;
    }

    cout << x << endl;

    core::md::fixed_array<int, 3, 3, 3> w{};
    for (auto i = 0; i < w.extent(0); ++i)
	for (auto j = 0; j < w.extent(1); ++j)
	    for (auto k = 0; k < w.extent(2); ++k)
		w[i, j, k] = i * w.extent(1) * w.extent(2) + j * w.extent(2) + k;
    cout << w << endl;

    cout << x.size() << endl;
    cout << a.size() << endl;
    cout << y.size() << endl;
    cout << z.size() << endl;
    cout << w.size() << endl;

    auto wx = w.to_mdspan();
    for (auto elem : wx)
	cout << elem << " ";
    
    return 0;
}
