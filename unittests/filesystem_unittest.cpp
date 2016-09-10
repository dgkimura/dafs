#include "gtest/gtest.h"

#include "filesystem.hpp"


TEST(BlockTest, testBlockFieldsAreConstructed)
{
    std::string contents = "the_block_contents";

    dafs::Block b(contents);

    ASSERT_EQ(contents, b.contents);
}
