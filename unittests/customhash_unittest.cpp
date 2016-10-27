#include <cstring>

#include "gtest/gtest.h"

#include "customhash.hpp"


TEST(CustomHashTest, testHashBlockFormat)
{
    dafs::BlockFormat b;

    // check empty hash
    std::memset(b.contents, 0, sizeof(char) * dafs::BLOCK_SIZE_IN_BYTES);
    ASSERT_EQ(0, std::hash<dafs::BlockFormat>{}(b));

    // write and verify hash
    std::memcpy(b.contents, "narf", 4);
    ASSERT_EQ(27, std::hash<dafs::BlockFormat>{}(b));

    // write to second half and verify new hash
    std::memcpy(b.contents + dafs::BLOCK_SIZE_IN_BYTES / 2, "zonk", 4);
    ASSERT_EQ(11, std::hash<dafs::BlockFormat>{}(b));
}
