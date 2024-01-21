// Copyright (C) 2023, 2024 by Mark Melton
//

#include <gtest/gtest.h>

#include "core/md/shared_data.h"
#include "core/md/compare.h"

using namespace core;
using ::testing::StaticAssertTypeEq;

TEST(SharedArray, Make) {
    auto array = md::make_shared_data<int>(17);
    EXPECT_EQ(array.extent(0), 17);
    for (auto i = 0; i < array.extent(0); ++i)
	array[i] = i;
    for (auto i = 0; i < array.extent(0); ++i)
	EXPECT_EQ(array[i], i);
}

TEST(SharedArray, MakeFromVec) {
    std::vector<std::string> data = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11"};
    auto a = md::make_shared_data(data, data.size());
    auto b = md::slice(a, md::x{0,20});
    EXPECT_TRUE(a == b);
    EXPECT_EQ(a.extent(0), data.size());
    for (auto i = 0; i < a.extent(0); ++i)
	EXPECT_EQ(std::atoi(a[i].c_str()), i);
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
