// Copyright (C) 2023 by Mark Melton
//

#include <gtest/gtest.h>
#include "core/md/multi_index.h"

using namespace core::md;
using ::testing::StaticAssertTypeEq;

TEST(MultiIndex, ConstructDefault)
{
    MultiIndex<int, 3> mdx{};
    static_assert(std::is_same_v<typename decltype(mdx)::IndexType, int>);
    static_assert(mdx.Rank == 3);
    EXPECT_TRUE(mdx.exhausted());
    EXPECT_EQ(mdx.index(), (std::array<int,3>{}));
}

template<std::integral T, size_t R> requires (R == 3)
void check_index(MultiIndex<T, R> mdx, const std::array<T, R>& extents) {
    auto mdx_orig = mdx;
    
    EXPECT_FALSE(mdx.exhausted());
    EXPECT_EQ(mdx.index(), (std::array{0, 0, 0}));
    EXPECT_EQ(mdx.extents(), extents);
    EXPECT_EQ(mdx.end_index(), (std::array{extents[0], 0, 0}));

    for (auto i = 0; i < mdx.extent(0); ++i) {
	for (auto j  = 0; j < mdx.extent(1); ++j) {
	    for (auto k = 0; k < mdx.extent(2); ++k) {
		EXPECT_EQ(mdx[0], i);
		EXPECT_EQ(mdx[1], j);
		EXPECT_EQ(mdx[2], k);
		++mdx;
	    }
	}
    }
    EXPECT_TRUE(mdx.exhausted());
    EXPECT_EQ(mdx.index(), mdx.end_index());

    auto end = mdx_orig.end_index();
    mdx_orig.advance_to_end();
    EXPECT_EQ(mdx_orig.index(), end);
}

TEST(MultiIndex, ConstructExtents)
{
    MultiIndex mdx(std::array{3, 4, 5});
    static_assert(std::is_same_v<typename decltype(mdx)::IndexType, int>);
    static_assert(mdx.Rank == 3);
    check_index(mdx, {3, 4, 5});
}

TEST(MultiIndex, ConstructExtentsPack)
{
    MultiIndex mdx(3, 4, 5);
    static_assert(std::is_same_v<typename decltype(mdx)::IndexType, int>);
    static_assert(mdx.Rank == 3);
    check_index(mdx, {3, 4, 5});
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
