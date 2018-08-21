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
        "t\n3\n\aa_block\x12&\n$00000000-0000-0000-0000-000000000000\x18"
    );
}


TEST(DiskTest, testAddBlockInNonemptyBlockFormat)
{
    std::stringstream stream(
        dafs::serialize(
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
        "\xF4z\n9\n\ranother_block\x12&\n$00000000-0000-0000-0000-000000000000\x18"
    );
}


TEST(DiskTest, testRemoveBlockInNonemptyBlockFormat)
{
    std::stringstream stream(
        dafs::serialize(
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
        "\xB4"
    );
}
