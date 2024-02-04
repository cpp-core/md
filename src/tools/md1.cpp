// Copyright (C) 2023, 2024 by Mark Melton
//

#include <algorithm>
#include <compare>
#include <iostream>
#include <ranges>
#include "core/md/array.h"
#include "core/md/span.h"
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

int main(int argc, const char *argv[]) {
    core::md::array<int, 1> x(12);
    cout << x.rank() << endl;
    cout << x.extent(0) << endl;
    // for (auto elem : x)
    // 	cout << elem << " ";
    // cout << endl;
    
    // core::md::shared<int, 1> v(12);
    // cout << v.rank() << endl;
    // cout << v.extent(0) << endl;
    // for (auto elem : v)
    // 	cout << elem << " ";
    // cout << endl;
}
