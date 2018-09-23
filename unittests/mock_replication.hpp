#pragma once

#include <vector>

#include <gtest/gtest.h>

#include "dafs/messages.hpp"
#include "dafs/replication.hpp"
#include "dafs/result.hpp"


class MockReplication : public dafs::Replication
{
public:

    MockReplication(std::vector<dafs::Address> missing_replicas={})
        : missing_replicas(missing_replicas)
    {
    }

    dafs::Result Write(std::string entry) override
    {
        dafs::Result r;
        entries.push_back(entry);
        return r;
    }

    void AddReplica(dafs::Address address, std::string location) override
    {
    }

    void RemoveReplica(dafs::Address address) override
    {
    }

    std::vector<dafs::Address> GetMissingReplicas() override
    {
        return missing_replicas;
    }

    int GetReplicaCount() override
    {
        return 3;
    }

    bool WasEntryWritten(std::string entry)
    {
        for (auto e : entries)
        {
            if (e == entry)
            {
                return true;
            }
        }
        return false;
    }

private:

    std::vector<dafs::Address> missing_replicas;

    std::vector<std::string> entries;
};
