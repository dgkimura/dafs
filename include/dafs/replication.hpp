#pragma once

#include <vector>

#include <paxos/parliament.hpp>

#include "dafs/commit.hpp"
#include "dafs/messages.hpp"
#include "dafs/signal.hpp"


namespace dafs
{
    class Replication
    {
    public:

        virtual void Write(std::string entry) = 0;

        virtual void AddReplica(dafs::Address address, std::string location) = 0;

        virtual void RemoveReplica(dafs::Address address) = 0;

        virtual std::vector<dafs::Address> GetMissingReplicas() = 0;
    };


    class PaxosReplication : public Replication
    {
    public:

        PaxosReplication(
            dafs::Address address,
            dafs::Root directory);

        PaxosReplication(
            const PaxosReplication& other);

        void Write(std::string entry) override;

        void AddReplica(dafs::Address address, std::string location) override;

        void RemoveReplica(dafs::Address address) override;

        std::vector<dafs::Address> GetMissingReplicas() override;

    private:

        std::shared_ptr<dafs::Signal> in_progress;

        Parliament parliament;
    };
}
