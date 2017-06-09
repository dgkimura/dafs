#include "gtest/gtest.h"

#include "dafs/filesystem.hpp"


TEST(FileSystemTest, testBlockFieldsAreConstructed)
{
    std::string contents = "the_block_contents";

    dafs::BlockFormat b;
    b.contents = contents;

    ASSERT_EQ(contents, b.contents);
}


TEST(FileSystemTest, testBytesFieldsAreConstructed)
{
    std::string data = "the_bytes_contents";

    dafs::Bytes b = dafs::CreateBytes(data);

    ASSERT_EQ(data, b.contents);
}


TEST(FileSystemTest, testsBlockInfoFieldsAreConstructed)
{
    std::string path = "myfile";
    dafs::Identity identity = dafs::Identity("01234567-89ab-cdef-0123-456789abcdef");

    dafs::BlockInfo b = dafs::BlockInfo{path, identity};

    ASSERT_EQ(path, b.path);
    ASSERT_EQ(identity, b.identity);
}


TEST(FileSystemTest, testsIdentityComparison)
{
    dafs::Identity a = dafs::Identity("00000000-0000-0000-0000-000000000000");
    dafs::Identity b = dafs::Identity("22222222-2222-2222-2222-222222222222");

    ASSERT_TRUE(a < b);
    ASSERT_FALSE(a < a);

    ASSERT_TRUE(a <= b);
    ASSERT_TRUE(a <= a);

    ASSERT_FALSE(a > b);
    ASSERT_FALSE(a > a);

    ASSERT_FALSE(a >= b);
    ASSERT_TRUE(a >= a);

    ASSERT_FALSE(a == b);
    ASSERT_TRUE(a == a);
}


TEST(FileSystemTest, testsSplitUpperIndexWithNoWrap)
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


TEST(FileSystemTest, testsSplitUpperIndexWithWrapAfterDivider)
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


TEST(FileSystemTest, testsSplitUpperIndexWithWrapAfterLower)
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


TEST(FileSystemTest, testsIdentityPostfixIncrement)
{
    dafs::Identity a = dafs::Identity("00000000-0000-0000-0000-000000000000");

    a += 1;
    ASSERT_EQ("00000000-0000-0000-0000-000000000001", a.id);

    a += 10;
    ASSERT_EQ("00000000-0000-0000-0000-00000000000b", a.id);

    a += 100;
    ASSERT_EQ("00000000-0000-0000-0000-00000000006f", a.id);

    a += 1000;
    ASSERT_EQ("00000000-0000-0000-0000-000000000457", a.id);

    a += 10000;
    ASSERT_EQ("00000000-0000-0000-0000-000000002b67", a.id);

    a += 100000;
    ASSERT_EQ("00000000-0000-0000-0000-00000001b207", a.id);

    a += 1000000;
    ASSERT_EQ("00000000-0000-0000-0000-00000010f447", a.id);

    a += 10000000;
    ASSERT_EQ("00000000-0000-0000-0000-000000a98ac7", a.id);

    a += 100000000;
    ASSERT_EQ("00000000-0000-0000-0000-0000069f6bc7", a.id);
}
