#include <vector>

#include "gtest/gtest.h"

#include "serialization.hpp"
#include "storage.hpp"


class TestableReplicatedStorage : public dafs::ReplicatedStorage
{
public:
    TestableReplicatedStorage()
    : ReplicatedStorage("", "localhost:1234", 1),
      log_of_writes()
    {
    }

    virtual dafs::BlockFormat ReadBlock(dafs::BlockInfo info)
    {
        dafs::BlockFormat b;
        dafs::BlockIndex index;
        b.contents = dafs::Serialize(index);
        return b;
    }

    virtual void do_write(dafs::ProposalType type, dafs::BlockInfo info, std::string data)
    {
        Write w
        {
            type,
            info,
            data
        };
        log_of_writes.push_back(w);
    }

    bool assert_write_called(dafs::ProposalType type, dafs::BlockInfo info, std::string data)
    {
        bool write_called = false;
        for (auto w : log_of_writes)
        {
            if (w.type == type && w.info.path == info.path && w.data == data)
            {
                write_called = true;
            }
        }
        return write_called;
    }

private:

    struct Write
    {
        dafs::ProposalType type;
        dafs::BlockInfo info;
        std::string data;
    };

    std::vector<Write> log_of_writes;
};


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


TEST(ReplicatedStorageTest, testCreateBlock)
{
    TestableReplicatedStorage store;
    store.CreateBlock(a_block);

    dafs::BlockIndex index;
    index.blocks.push_back(a_block);

    ASSERT_TRUE(
        store.assert_write_called(
            dafs::ProposalType::CreateBlock,
            the_blocklist,
            "22 serialization::archive 11 0 0 7 a_block 0 0 10 an_address 8080 0")
    );
}
