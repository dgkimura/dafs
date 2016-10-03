#include "gtest/gtest.h"

#include "delta.hpp"


TEST(DeltaTest, testDeltaFactorBasicCreate)
{
    dafs::Delta d = dafs::DeltaFactory::Create("a_filename", "abcdef", "ace");

    ASSERT_EQ("a_filename", d.filename);
    ASSERT_EQ("\xC0\x80\xC0\x80\xC0\x80", d.difference);
}
