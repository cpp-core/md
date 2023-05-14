// Copyright (C) 2023 by Mark Melton
//

#include <iostream>
#include "core/md/array.h"
#include "core/md/span.h"
#include "core/md/shared.h"
#include "core/md/ostream.h"

using std::cout, std::endl;

int main(int argc, const char *argv[]) {
    core::md::array<int, 2> x(6, 10);
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
    
    return 0;
}
