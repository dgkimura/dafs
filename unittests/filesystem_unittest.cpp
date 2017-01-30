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

    dafs::BlockInfo b = dafs::CreateBlockInfo(path, identity);

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


TEST(FileSystemTest, testsIdentityArithmatic)
{
    dafs::Identity a = dafs::Identity("33333333-3333-3333-3333-333333333333");
    dafs::Identity b = dafs::Identity("11111111-1111-1111-1111-111111111111");

    // test arithmatic
    ASSERT_EQ(dafs::Identity("44444444-4444-4444-4444-444444444444"), a + b);
    ASSERT_EQ(dafs::Identity("22222222-2222-2222-2222-222222222222"), a - b);

    a = dafs::Identity("00000000-0000-0000-0000-000000000991");
    b = dafs::Identity("00000000-0000-0000-0000-000000000088");

    // test carrying arithmatic
    ASSERT_EQ(dafs::Identity("00000000-0000-0000-0000-000000000a19"), a + b);
    ASSERT_EQ(dafs::Identity("00000000-0000-0000-0000-000000000909"), a - b);
}


TEST(FileSystemTest, testsIdentityMedian)
{
    dafs::Identity a = dafs::Identity("33333333-3333-3333-3333-333333333333");
    dafs::Identity b = dafs::Identity("11111111-1111-1111-1111-111111111111");

    ASSERT_EQ(dafs::Identity("22222222-2222-2222-2222-222222222222"), a.Median(b));

    a = dafs::Identity("ffffffff-ffff-ffff-ffff-ffffffffffff");
    b = dafs::Identity("11111111-1111-1111-1111-111111111111");

    ASSERT_EQ(dafs::Identity("88888888-8888-8888-8888-888888888888"), a.Median(b));
}
