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
            interface
        };
    }

    virtual dafs::Identity GetIdentity() override
    {
        return identity;
    }

    virtual dafs::ReplicatedEndpoints GetNodeSetDetails() override
    {
        return dafs::ReplicatedEndpoints
        {
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

    virtual void RemoveNode(dafs::Address address) override
    {
    }

    virtual void SetMinus(
        dafs::Address management,
        dafs::Address replication,
        std::string location) override
    {
    }

    virtual void SetZero(
        dafs::Address management,
        dafs::Address replication,
        std::string location) override
    {
    }

    virtual void SetPlus(
        dafs::Address management,
        dafs::Address replication,
        std::string location) override
    {
    }

    virtual std::vector<dafs::Address> NonresponsiveMembers(
        int minimum_elections) override
    {
        return std::vector<dafs::Address>{};
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
