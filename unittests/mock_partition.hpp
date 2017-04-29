#pragma once

#include <unordered_map>

#include "dafs/partition.hpp"


class MockPartition : public dafs::Partition
{
public:

    MockPartition(
        dafs::Endpoint minus,
        dafs::Endpoint zero,
        dafs::Endpoint plus,
        dafs::Identity identity
    )
        : minus(minus),
          zero(zero),
          plus(plus),
          identity(identity),
          is_active(true)
    {
    }

    virtual dafs::Identity GetIdentity() override
    {
        return identity;
    }

    virtual dafs::ReplicatedEndpoints GetNodeSetDetails() override
    {
        return dafs::ReplicatedEndpoints
        {
            minus,
            zero,
            plus
        };
    }

    virtual bool IsAddressResponsive(dafs::Address address) override
    {
        return true;
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

    virtual void SetMinus(
        dafs::Address management,
        dafs::Address replication,
        std::string location) override
    {
        minus.management = management;
        minus.replication = replication;
    }

    virtual void SetZero(
        dafs::Address management,
        dafs::Address replication,
        std::string location) override
    {
        zero.management = management;
        zero.replication = replication;
    }

    virtual void SetPlus(
        dafs::Address management,
        dafs::Address replication,
        std::string location) override
    {
        plus.management = management;
        plus.replication = replication;
    }

    virtual bool IsActive() override
    {
        return is_active;
    }

private:

    dafs::Endpoint minus;

    dafs::Endpoint zero;

    dafs::Endpoint plus;

    dafs::Identity identity;

    bool is_active;

    std::unordered_map<std::string, dafs::BlockFormat> block_map;
};
