#include <sstream>

#include <gtest/gtest.h>

#include "dafs/delta.hpp"
#include "dafs/identity.hpp"
#include "dafs/blocks.hpp"
#include "dafs/serialization.hpp"


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


TEST(DeltaTest, testCreateDeltaAndApplyDeltaOnBlockIndex)
{
    dafs::BlockIndex prev
    {
        {
            dafs::BlockInfo{"10000001-0000-0000-0000-000000000000", dafs::Identity("10000001-0000-0000-0000-000000000000"), 1}
        }
    };
    dafs::BlockIndex next
    {
        {
            dafs::BlockInfo{"10000001-0000-0000-0000-000000000000", dafs::Identity("10000001-0000-0000-0000-000000000000"), 1},
            dafs::BlockInfo{"10000001-0000-0000-0000-000000000001", dafs::Identity("10000001-0000-0000-0000-000000000001"), 1},
        }
    };

    auto delta = dafs::CreateDelta(dafs::serialize(prev), dafs::serialize(next));
    std::stringstream ss;
    ss << dafs::serialize(prev);
    std::string updated_content = dafs::ApplyDelta(delta, ss);
    dafs::BlockIndex updated_blockindex = dafs::deserialize<dafs::BlockIndex>(updated_content);
    ASSERT_EQ(2, updated_blockindex.items.size());
}
