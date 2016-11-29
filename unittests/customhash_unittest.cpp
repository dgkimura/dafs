#include <cstring>

#include "gtest/gtest.h"

#include "customhash.hpp"


TEST(CustomHashTest, testHashBlockFormat)
{
    dafs::BlockFormat b;

    // check empty hash
    b.contents = "\0";
    ASSERT_EQ(0, std::hash<dafs::BlockFormat>{}(b));

    // write and verify hash
    b.contents = "narf";
    ASSERT_EQ(27, std::hash<dafs::BlockFormat>{}(b));

    // write to second half and verify new hash
    b.contents.insert(3, "zonk");
    ASSERT_EQ(11, std::hash<dafs::BlockFormat>{}(b));
}
