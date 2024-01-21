// Copyright (C) 2023, 2024 by Mark Melton
//

#include <variant>
#include <array>
#include <gtest/gtest.h>

#include "core/md/span.h"
#include "core/md/compare.h"

using namespace core;
using ::testing::StaticAssertTypeEq;

TEST(Span, Construct1d) {
    std::array<int, 7> arr = {0, 1, 2, 3, 4, 5, 6};
    md::span<int, 1> x(arr.data(), arr.size());
    EXPECT_EQ(x.rank(), 1);
    EXPECT_EQ(x.extent(0), arr.size());
    for (auto i = 0; i < arr.size(); ++i)
        EXPECT_EQ(x[i], arr[i]);
}

TEST(Span, Construct2d) {
    std::array<int, 12> arr = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    md::span<int, 2> x(arr.data(), 3, 4);
    EXPECT_EQ(x.rank(), 2);
    EXPECT_EQ(x.extent(0), 3);
    EXPECT_EQ(x.extent(1), 4);
    for (auto i = 0; i < arr.size(); ++i)
        for (auto j = 0; j < arr.size(); ++j)
            EXPECT_EQ((x[i, j]), arr[i * 4 + j]);
}

TEST(Span, Construct3d) {
    std::array<int, 12> arr = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
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

TEST(Span, Slice1d) {
    std::vector v0 = {0, 1, 2, 3, 4, 5, 6};
    md::span<int, 1> x(v0.data(), v0.size());
    
    auto xs0 = md::slice(x, md::all);
    EXPECT_TRUE(x == xs0);
    
    auto xs1 = md::slice(x, md::x{2, 5});
    std::vector v1 = {2, 3, 4};
    md::span<int, 1> x1(v1.data(), v1.size());
    EXPECT_TRUE(xs1 == x1);
    
    auto xs2 = md::slice(x, md::x{0, -3});
    std::vector v2 = {0, 1, 2, 3};
    md::span<int, 1> x2(v2.data(), v2.size());
    EXPECT_TRUE(xs2 == x2);
}

TEST(Span, Slice2d) {
    std::array<int, 12> arr = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    md::span<int, 2> x(arr.data(), 3, 4);
    auto xs0 = md::slice(x, md::x{0, 1}, md::all);
    EXPECT_EQ(xs0.extent(0), 1);
    EXPECT_EQ(xs0.extent(1), 4);
    
    auto xs1 = md::slice(x, md::x{0, 2}, md::x{0, 3});
    EXPECT_EQ(xs1.extent(0), 2);
    EXPECT_EQ(xs1.extent(1), 3);
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
