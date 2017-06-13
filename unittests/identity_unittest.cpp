#include "gtest/gtest.h"

#include "dafs/identity.hpp"


TEST(IdentityTest, testsIdentityComparison)
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
