#include "gtest/gtest.h"

#include "filesystem.hpp"


TEST(BlockTest, testBlockFieldsAreConstructed)
{
    dafs::Block b =
    {
        1,
        0,
        "the_owner",
        "the_block_contents"
    };

    ASSERT_EQ(b.id, 1);
    ASSERT_EQ(b.revision, 0);
    ASSERT_EQ(b.owner, "the_owner");
    ASSERT_EQ(b.contents, "the_block_contents");
}
