#include <functional>
#include <vector>

#include "gtest/gtest.h"

#include "dafs/serialization.hpp"
#include "dafs/disk.hpp"


const dafs::BlockInfo the_blocklist
{
    "blocklist",
    dafs::Identity
    {
        "00000000-0000-0000-0000-000000000000",
    },
    0
};


const dafs::BlockInfo a_block
{
    "a_block",
    dafs::Identity
    {
        "00000000-0000-0000-0000-000000000000",
    },
    0
};


const dafs::BlockInfo another_block
{
    "another_block",
    dafs::Identity
    {
        "00000000-0000-0000-0000-000000000000",
    },
    0
};


TEST(DiskTest, testAddBlockInEmptyBlockFormat)
{
    std::stringstream stream;

    dafs::Delta delta = dafs::Insert(stream, a_block);
    ASSERT_EQ(
        delta.difference,
        "\xE4" "A1 \xC2y 0 7 a_block 0 0 36 00000000-0000-0000-0000-000000000000 0"
    );
}


TEST(DiskTest, testAddBlockInNonemptyBlockFormat)
{
    std::stringstream stream(
        dafs::Serialize(
            dafs::BlockIndex
            {
                std::vector<dafs::BlockInfo>
                {
                    a_block
                }
            }
        )
    );

    dafs::Delta delta = dafs::Insert(stream, another_block);
    ASSERT_EQ(
        delta.difference,
        "\xE4\x80@2\xFDz 13 another_block 36 00000000-0000-0000-0000-000000000000 0"
    );
}


TEST(DiskTest, testRemoveBlockInNonemptyBlockFormat)
{
    std::stringstream stream(
        dafs::Serialize(
            dafs::BlockIndex
            {
                std::vector<dafs::BlockInfo>
                {
                    a_block
                }
            }
        )
    );

    dafs::Delta delta = dafs::Remove(stream, a_block);
    ASSERT_EQ(
        delta.difference,
        "\xE4\x81\xC2\xB9"
    );
}
