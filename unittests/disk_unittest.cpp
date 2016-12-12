#include <functional>
#include <vector>

#include "gtest/gtest.h"

#include "serialization.hpp"
#include "disk.hpp"


const dafs::BlockInfo the_blocklist
{
    "blocklist",
    dafs::Location
    {
        "localhost",
        8080
    },
    0
};


const dafs::BlockInfo a_block
{
    "a_block",
    dafs::Location
    {
        "an_address",
        8080
    },
    0
};


const dafs::BlockInfo another_block
{
    "another_block",
    dafs::Location
    {
        "an_address",
        8080
    },
    0
};


TEST(DiskTest, testAddBlockInEmptyBlockFormat)
{
    auto get_empty_block = [](dafs::BlockInfo info) {
        dafs::BlockFormat b;
        return b;
    };

    dafs::Delta delta = Insert(the_blocklist, a_block, get_empty_block);
    ASSERT_EQ(
        delta.difference,
        "\xE0@7\xC0\x80\xE1" "A1 \xC0" "d 0 7 a_block 0 0 10 an_address 8080 0"
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

    dafs::Delta delta = Insert(the_blocklist, another_block, get_block);
    ASSERT_EQ(
        delta.difference,
        "\xE0\x81" "B111\xE1\x80@2\xE6" "e 13 another_block 10 an_address 8080 0"
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

    dafs::Delta delta = Remove(the_blocklist, a_block, get_block);
    ASSERT_EQ(
        delta.difference,
        "\xE0\x80\xC0@4\xE1\x81\xC0\xA4"
    );
}
