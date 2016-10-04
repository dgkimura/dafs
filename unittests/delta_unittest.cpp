#include "gtest/gtest.h"

#include "delta.hpp"


TEST(DeltaTest, testCreateDeltaBasic)
{
    dafs::Delta d = dafs::CreateDelta("a_filename", "abcdef", "ace");

    ASSERT_EQ("a_filename", d.filename);
    ASSERT_EQ("\xC0\x80\xC0\x80\xC0\x80", d.difference);
}


TEST(DeltaTest, testApplyDeltaBasic)
{
    dafs::Delta d = dafs::CreateDelta("a_filename", "abcdef", "ace");

    std::string expect = "ace";
    std::string actual = dafs::ApplyDelta(d, "abcdef");

    for (int i=0; i<expect.length(); i++)
    {
        ASSERT_EQ(expect[i], actual[i]);
    }
}
