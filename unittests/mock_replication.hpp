#pragma once

#include <vector>

#include "dafs/messages.hpp"
#include "dafs/replication.hpp"
#include "dafs/signal.hpp"


class MockReplication : public dafs::Replication
{
public:

    MockReplication(std::shared_ptr<dafs::Signal> signal,
                    std::vector<dafs::Address> missing_replicas={})
        : signal(signal),
          missing_replicas(missing_replicas)
    {
    }

    void Write(std::string entry) override
    {
        signal->Set();
    }

    void AddReplica(dafs::Address address, std::string location) override
    {
        signal->Set();
    }

    void RemoveReplica(dafs::Address address) override
    {
        signal->Set();
    }

    std::vector<dafs::Address> GetMissingReplicas() override
    {
        return missing_replicas;
    }

private:

    std::shared_ptr<dafs::Signal> signal;

    std::vector<dafs::Address> missing_replicas;
};
