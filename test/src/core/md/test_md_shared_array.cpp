// Copyright (C) 2023, 2024 by Mark Melton
//

#include <variant>
#include <array>
#include <gtest/gtest.h>

#include "core/md/shared.h"
#include "core/md/compare.h"

using namespace core;
using ::testing::StaticAssertTypeEq;

TEST(SharedArray, Construct1d) {
    md::shared_array<int, 1> array(17);
    EXPECT_EQ(array.extent(0), 17);
    for (auto i = 0; i < array.extent(0); ++i)
	array[i] = i;
    for (auto i = 0; i < array.extent(0); ++i)
	EXPECT_EQ(array[i], i);
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
