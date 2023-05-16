// Copyright (C) 2023 by Mark Melton
//

#include <gtest/gtest.h>
#include <array>
#include "core/md/span.h"

using namespace core;
using ::testing::StaticAssertTypeEq;

TEST(Span, Construct1d)
{
    std::array<int, 7> arr = { 0, 1, 2, 3, 4, 5, 6 };
    md::span<int, 1> x(arr.data(), arr.size());
    EXPECT_EQ(x.rank(), 1);
    EXPECT_EQ(x.extent(0), arr.size());
    for (auto i = 0; i < arr.size(); ++i)
	EXPECT_EQ(x[i], arr[i]);
}

TEST(Span, Construct2d)
{
    std::array<int, 12> arr = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
    md::span<int, 2> x(arr.data(), 3, 4);
    EXPECT_EQ(x.rank(), 2);
    EXPECT_EQ(x.extent(0), 3);
    EXPECT_EQ(x.extent(1), 4);
    for (auto i = 0; i < arr.size(); ++i)
	for (auto j = 0; j < arr.size(); ++j)
	    EXPECT_EQ((x[i, j]), arr[i * 4 + j]);
}

TEST(Span, Construct3d)
{
    std::array<int, 12> arr = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
    md::span<int, 3> x(arr.data(), 3, 2, 2);
    EXPECT_EQ(x.rank(), 3);
    EXPECT_EQ(x.extent(0), 3);
    EXPECT_EQ(x.extent(1), 2);
    EXPECT_EQ(x.extent(2), 2);
    for (auto i = 0; i < arr.size(); ++i)
	for (auto j = 0; j < arr.size(); ++j)
	    for (auto k = 0; k < arr.size(); ++k)
		EXPECT_EQ((x[i, j, k]), arr[i * 4 + j * 2 + k]);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
