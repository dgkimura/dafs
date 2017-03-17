#pragma once

#include <unordered_map>

#include "dafs/partition.hpp"


class MockPartition : public dafs::Partition
{
public:

    MockPartition(
        dafs::Address author,
        dafs::Address interface,
        dafs::Identity identity
    )
        : author(author),
          interface(interface),
          identity(identity),
          is_active(true)
    {
    }

    virtual dafs::PartitionDetails GetDetails() override
    {
        return dafs::PartitionDetails
        {
            author,
            interface,
            identity
        };
    }

    virtual void DeleteBlock(dafs::BlockInfo block) override
    {
        block_map.erase(block.identity.id);
    }

    virtual dafs::BlockFormat ReadBlock(dafs::BlockInfo block) override
    {
        return block_map[block.identity.id];
    }

    virtual void WriteBlock(dafs::BlockInfo block, dafs::BlockFormat format) override
    {
        block_map[block.identity.id] = format;
    }

    virtual void AddNode(dafs::Address address, std::string location) override
    {
    }

    virtual void RemoveNode(dafs::Address address) override
    {
    }

    virtual bool IsActive() override
    {
        return is_active;
    }

private:

    dafs::Address author;

    dafs::Address interface;

    dafs::Identity identity;

    bool is_active;

    std::unordered_map<std::string, dafs::BlockFormat> block_map;
};
