#include "gtest/gtest.h"

#include "filesystem.hpp"


TEST(BlockTest, testBlockFieldsAreConstructed)
{
    std::string contents = "the_block_contents";

    dafs::BlockFormat b;
    strcpy(b.contents, contents.c_str());

    ASSERT_EQ(contents, b.contents);
}
