// Copyright (C) 2023 by Mark Melton
//

#include <iostream>
#include "core/md/array.h"
#include "core/md/span.h"
#include "core/md/shared.h"
#include "core/md/ostream.h"
#include "core/md/concept.h"

namespace core::md {

template<std::integral T, size_t N>
class IndexCounter {
    template<std::integral... Ts>
    IndexCounter(Ts... indices)
	: last_(static_cast<T>(indices)...) {
    }

    const auto& index() const {
	return index_;
    }

    void increment() {
    }
    
    IndexCounter& operator++() {
	increment();
	return *this;
    }

private:
    std::array<T, N> index_{}, last_{};
};

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
    
    span_iterator(T& span)
	: span_(span) {
	initialize_last_index();
    }

    reference operator*() const {
	return span_[index_];
    }

    pointer operator->() const {
	return &span_[index_];
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
	for (auto i = 0; i < index_.size(); ++i)
	    if (index_[i] != last_[i])
		return false;
	return true;
    }

private:
    using index_seq = std::index_sequence<Rank>;
    
    void initialize_last_index() {
	for (auto i = 0; i < Rank; ++i)
	    last_[i] = span_.extent(i);
    }

    void increment() {
	for (int i = index_.size() - 1; i >= 0; --i) {
	    if (index_[i] < last_[i]) {
		++index_[i];
		break;
	    }
	    index_[i] = 0;
	}
    }

    T& span_;
    std::array<index_type, Rank> index_, last_;
};

}; // core::md

using std::cout, std::endl;

template<core::md::Span T, core::md::Span U>
requires (T::extents_type::rank() == U::extents_type::rank())
constexpr bool operator==(const T& a, const U& b) {
    using E = typename T::extents_type;
    constexpr auto rank = E::rank();
    for (auto i = 0; i < rank; ++i)
	if (a.extent(i) != b.extent(i))
	    return false;
    if constexpr (rank == 1) {
	for (auto i = 0; i < a.extent(0); ++i)
	    if (a[i] != b[i])
		return false;
    }
    else if constexpr (rank == 2) {
	for (auto i = 0; i < a.extent(0); ++i)
	    for (auto j = 0; j < a.extent(1); ++j)
		if (a[i, j] != b[i, j])
		    return false;
    }
    return true;
}

int main(int argc, const char *argv[]) {
    core::md::array<int, 2> x(6, 10);
    core::md::array<int, 2> a(6, 10);
    assert(a == x);
    
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
    core::md::span_iterator iter(wx);
    decltype(iter)::sentinel sentinel{};
    while (iter != sentinel) {
	cout << *iter << " ";
	++iter;
    }
    cout << endl;
    
    return 0;
}
