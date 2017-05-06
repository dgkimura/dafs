#pragma once

#include <vector>

#include "dafs/messages.hpp"
#include "dafs/replication.hpp"


class MockReplication : public dafs::Replication
{
public:

    MockReplication(std::vector<dafs::Address> missing_replicas={})
        : missing_replicas(missing_replicas)
    {
    }

    void Write(std::string entry) override
    {
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

private:

    std::vector<dafs::Address> missing_replicas;
};
