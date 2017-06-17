#include <sstream>

#include <gtest/gtest.h>

#include "dafs/delta.hpp"


TEST(DeltaTest, testCreateDeltaBasic)
{
    dafs::Delta d = dafs::CreateDelta("abcdef", "ace");

    ASSERT_EQ("\xC0\x80\xC0\x80\xC0\x80", d.difference);
}


TEST(DeltaTest, testApplyDeltaBasic)
{
    dafs::Delta d = dafs::CreateDelta("abcdef", "ace");

    std::string expect = "ace";
    std::stringstream stream;
    stream << "abcdef";
    std::string actual = dafs::ApplyDelta(d, stream);

    for (int i=0; i<expect.length(); i++)
    {
        ASSERT_EQ(expect[i], actual[i]);
    }
}
