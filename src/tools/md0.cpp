// Copyright (C) 2023, 2024 by Mark Melton
//

#undef NDEBUG

#include <algorithm>
#include <compare>
#include <iostream>
#include <ranges>
#include "core/md/array.h"
#include "core/md/span.h"
#include "core/md/shared.h"
#include "core/md/ostream.h"
#include "core/md/multi_index.h"
#include "core/md/index_iterator.h"
#include "core/md/iterator.h"
#include "core/md/compare.h"

namespace core::md {

static_assert(std::forward_iterator<index_iterator<int,2>>);
static_assert(std::forward_iterator<index_iterator<int,2>>);

static_assert(std::forward_iterator<span_iterator<array<int,2>>>);
static_assert(std::forward_iterator<span_iterator<span<int,2>>>);

}; // core::md

using std::cout, std::endl;

using namespace core::md;

int main(int argc, const char *argv[]) {
    core::md::array<int, 2> x(6, 10);
    core::md::array<int, 2> a(6, 10);

    auto xs = x.to_mdspan();
    auto as = a.to_mdspan();

    for (auto i : std::ranges::iota_view{1, 10})
	cout << i << " ";
    cout << endl;

    std::ranges::fill(begin(element_wise{xs}), end(element_wise{xs}), 42);
    std::ranges::fill(begin(element_wise{as}), end(element_wise{as}), 42);
    assert(a.to_mdspan() == x.to_mdspan());
    assert(xs == as);
    
    
    // core::md::fixed_array<int, 3, 3> y{};
    // core::md::shared<int, 2> z(7, 8);

    // std::ranges::generate(begin(xs), end(xs), [n=0]() mutable { return n++; });
    // cout << "generate: " << endl << xs << endl;

    // std::ranges::for_each(begin_index(xs), end_index(xs), [&](const auto& index) {
    // 	xs[index[0], index[1]] = index[0] * xs.extent(1) + index[1];
    // });
    // cout << "for_each: " << endl << xs << endl;

    // core::md::fixed_array<int, 3, 3, 3> w{};
    // auto wx = w.to_mdspan();
    // std::ranges::generate(begin(wx), end(wx), [n=0]() mutable { return n++; });
    // cout << w << endl;

    // cout << x.size() << endl;
    // cout << a.size() << endl;
    // cout << y.size() << endl;
    // cout << z.size() << endl;
    // cout << w.size() << endl;

    // for (auto elem : wx)
    // 	cout << elem << " ";
    
    return 0;
}
