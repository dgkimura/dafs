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
    auto get_empty_block = [](dafs::BlockInfo info) {
        dafs::BlockFormat b;
        return b;
    };

    dafs::Delta delta = dafs::Insert(the_blocklist, a_block, get_empty_block);
    ASSERT_EQ(
        delta.difference,
        "\x7F" "22 serialization::archive 14 0 0 1 0 0 7 a_block 0 0 36 00000000]-0000-0000-0000-000000000000 0"
    );
}


TEST(DiskTest, testAddBlockInNonemptyBlockFormat)
{
    auto get_block = [](dafs::BlockInfo info) -> dafs::BlockFormat {
        dafs::BlockFormat block;
        block.contents = dafs::Serialize(
            dafs::BlockIndex
            {
                std::vector<dafs::BlockInfo>
                {
                    a_block
                }
            }
        );
        return block;
    };

    dafs::Delta delta = dafs::Insert(the_blocklist, another_block, get_block);
    ASSERT_EQ(
        delta.difference,
        "\xE0\x80@2\xFBz 13 another_block 36 00000000-0000-0000-0000-000000000000 0"
    );
}


TEST(DiskTest, testRemoveBlockInNonemptyBlockFormat)
{
    auto get_block = [](dafs::BlockInfo info) -> dafs::BlockFormat {
        dafs::BlockFormat block;
        block.contents = dafs::Serialize(
            dafs::BlockIndex
            {
                std::vector<dafs::BlockInfo>
                {
                    a_block
                }
            }
        );
        return block;
    };

    dafs::Delta delta = dafs::Remove(the_blocklist, a_block, get_block);
    ASSERT_EQ(
        delta.difference,
        "\xE0\x81\xC0\xB9"
    );
}
