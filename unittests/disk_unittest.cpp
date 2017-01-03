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

    dafs::Delta delta = dafs::Insert(the_blocklist, a_block, get_empty_block);
    ASSERT_EQ(
        delta.difference,
        "\x7F" "22 serialization::archive 11 0 0 1 0 0 7 a_block 0 0 10 an_addreHss 8080 0"
    );
}


TEST(DiskTest, testSetBlockInEmptyBlockFormat)
{
    auto get_empty_block = [](dafs::BlockInfo info) {
        dafs::BlockFormat b;
        b.contents = dafs::Serialize(1);
        return b;
    };

    std::string i = "3";
    dafs::Delta delta = dafs::Set(the_blocklist, i, get_empty_block);
    ASSERT_EQ(
        delta.difference,
        "\x9D@3"
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
        "\xE0\x80@2\xE6" "e 13 another_block 10 an_address 8080 0"
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
        "\xE0\x81\xC0\xA4"
    );
}
