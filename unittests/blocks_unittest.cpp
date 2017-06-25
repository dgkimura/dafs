#include "gtest/gtest.h"

#include "dafs/blocks.hpp"


TEST(BlocksTest, testRootFieldsAreConstructed)
{
    std::string directory = "a_directory";

    dafs::Root root(directory);

    ASSERT_EQ(directory, root.directory);
}


TEST(BlocksTest, testBlockFieldsAreConstructed)
{
    std::string contents = "the_block_contents";

    dafs::BlockFormat b;
    b.contents = contents;

    ASSERT_EQ(contents, b.contents);
}


TEST(BlocksTest, testsBlockInfoFieldsAreConstructed)
{
    std::string path = "myfile";
    dafs::Identity identity = dafs::Identity("01234567-89ab-cdef-0123-456789abcdef");

    dafs::BlockInfo b = dafs::BlockInfo{path, identity};

    ASSERT_EQ(path, b.path);
    ASSERT_EQ(identity, b.identity);
}


TEST(BlocksTest, testsSplitUpperIndexWithNoWrap)
{
    auto index = SplitUpperIndex(
        dafs::BlockIndex
        {
            std::vector<dafs::BlockInfo>
            {
                dafs::BlockInfo
                {
                    "a_block",
                    dafs::Identity("10000000-0000-0000-0000-000000000000"),
                    0
                },
                dafs::BlockInfo
                {
                    "b_block",
                    dafs::Identity("10000000-0000-0000-0000-222222222222"),
                    0
                },
                dafs::BlockInfo
                {
                    "c_block",
                    dafs::Identity("10000000-0000-0000-0000-444444444444"),
                    0
                }
            }
        },
        dafs::Identity("10000000-0000-0000-0000-111111111111"),
        dafs::Identity("10000000-0000-0000-0000-333333333333"),
        dafs::Identity("10000000-0000-0000-0000-555555555555")
    );

    ASSERT_EQ("10000000-0000-0000-0000-444444444444", index.items[0].identity.id);
}


TEST(BlocksTest, testsSplitUpperIndexWithWrapAfterDivider)
{
    auto index = SplitUpperIndex(
        dafs::BlockIndex
        {
            std::vector<dafs::BlockInfo>
            {
                dafs::BlockInfo
                {
                    "a_block",
                    dafs::Identity("10000000-0000-0000-0000-000000000000"),
                    0
                },
                dafs::BlockInfo
                {
                    "b_block",
                    dafs::Identity("10000000-0000-0000-0000-222222222222"),
                    0
                },
                dafs::BlockInfo
                {
                    "c_block",
                    dafs::Identity("10000000-0000-0000-0000-444444444444"),
                    0
                }
            }
        },
        dafs::Identity("10000000-0000-0000-0000-333333333333"),
        dafs::Identity("10000000-0000-0000-0000-555555555555"),
        dafs::Identity("10000000-0000-0000-0000-111111111111")
    );

    ASSERT_EQ("10000000-0000-0000-0000-000000000000", index.items[0].identity.id);

    index = SplitUpperIndex(
        dafs::BlockIndex
        {
            std::vector<dafs::BlockInfo>
            {
                dafs::BlockInfo
                {
                    "c_block",
                    dafs::Identity("10000000-0000-0000-0000-666666666666"),
                    0
                }
            }
        },
        dafs::Identity("10000000-0000-0000-0000-333333333333"),
        dafs::Identity("10000000-0000-0000-0000-555555555555"),
        dafs::Identity("10000000-0000-0000-0000-111111111111")
    );

    ASSERT_EQ("10000000-0000-0000-0000-666666666666", index.items[0].identity.id);
}


TEST(BlocksTest, testsSplitUpperIndexWithWrapAfterLower)
{
    auto index = SplitUpperIndex(
        dafs::BlockIndex
        {
            std::vector<dafs::BlockInfo>
            {
                dafs::BlockInfo
                {
                    "a_block",
                    dafs::Identity("10000000-0000-0000-0000-000000000000"),
                    0
                },
                dafs::BlockInfo
                {
                    "b_block",
                    dafs::Identity("10000000-0000-0000-0000-222222222222"),
                    0
                },
                dafs::BlockInfo
                {
                    "c_block",
                    dafs::Identity("10000000-0000-0000-0000-444444444444"),
                    0
                }
            }
        },
        dafs::Identity("10000000-0000-0000-0000-555555555555"),
        dafs::Identity("10000000-0000-0000-0000-111111111111"),
        dafs::Identity("10000000-0000-0000-0000-333333333333")
    );

    ASSERT_EQ("10000000-0000-0000-0000-222222222222", index.items[0].identity.id);
}
