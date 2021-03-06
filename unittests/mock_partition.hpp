#pragma once

#include <unordered_map>

#include "dafs/partition.hpp"


class MockPartition : public dafs::Partition
{
public:

    MockPartition(
        dafs::Endpoint minus,
        dafs::Endpoint zero,
        dafs::Endpoint plus
    )
        : minus(minus),
          zero(zero),
          plus(plus),
          is_active(true)
    {
    }

    virtual dafs::Identity GetIdentity() override
    {
        return zero.identity;
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

    virtual dafs::BlockInfo AllocateBlock() override
    {
        dafs::BlockInfo info;
        dafs::BlockFormat format;
        while (block_map.find(info) != block_map.end())
        {
            info = dafs::BlockInfo
            {
                "",
                boost::uuids::to_string(boost::uuids::random_generator()()),
                0
            };
        }
        block_map[info] = format;
        return info;
    }

    virtual void DeleteBlock(dafs::BlockInfo block) override
    {
        block_map.erase(block);
    }

    virtual dafs::BlockFormat ReadBlock(dafs::BlockInfo block) override
    {
        return block_map[block];
    }

    virtual void WriteBlock(dafs::BlockInfo block, dafs::BlockFormat format) override
    {
        block_map[block] = format;
    }

    virtual dafs::BlockIndex GetIndex() override
    {
        std::vector<dafs::BlockInfo> keys;
        for (auto i : block_map)
        {
            keys.push_back(i.first);
        }
        return dafs::BlockIndex
        {
            keys
        };
    }

    virtual void SetMinus(
        dafs::Address management,
        dafs::Address replication,
        dafs::Identity identity,
        std::string fault_domain,
        std::string location) override
    {
        minus.management = management;
        minus.replication = replication;
    }

    virtual void SetZero(
        dafs::Address management,
        dafs::Address replication,
        dafs::Identity identity,
        std::string fault_domain,
        std::string location) override
    {
        zero.management = management;
        zero.replication = replication;
    }

    virtual void SetPlus(
        dafs::Address management,
        dafs::Address replication,
        dafs::Identity identity,
        std::string fault_domain,
        std::string location) override
    {
        plus.management = management;
        plus.replication = replication;
    }

    virtual bool IsActive() override
    {
        return is_active;
    }

    virtual bool Acquire() override
    {
        return true;
    }

    virtual void Release() override
    {
    }

private:

    dafs::Endpoint minus;

    dafs::Endpoint zero;

    dafs::Endpoint plus;

    bool is_active;

    std::unordered_map<dafs::BlockInfo, dafs::BlockFormat> block_map;
};
