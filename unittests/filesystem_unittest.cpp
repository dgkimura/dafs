#include "gtest/gtest.h"

#include "filesystem.hpp"


TEST(BlockTest, testBlockFieldsAreConstructed)
{
    int id = 1;
    int revision = 0;
    std::string owner = "the_owner";
    std::string contents = "the_block_contents";

    dafs::Block b(id, revision, owner, contents);

    ASSERT_EQ(id, b.id);
    ASSERT_EQ(revision, b.revision);
    ASSERT_EQ(owner, b.owner);
    ASSERT_EQ(contents, b.contents);
}
