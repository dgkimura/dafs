#pragma once

#include <mutex>
#include <vector>

#include <boost/uuid/uuid.hpp>
#include <paxos/parliament.hpp>

#include "dafs/customhash.hpp"
#include "dafs/commit.hpp"
#include "dafs/messages.hpp"
#include "dafs/signal.hpp"


namespace dafs
{
    class Replication
    {
    public:

        virtual dafs::Result Write(std::string entry) = 0;

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

        dafs::Result Write(std::string entry) override;

        void AddReplica(dafs::Address address, std::string location) override;

        void RemoveReplica(dafs::Address address) override;

        std::vector<dafs::Address> GetMissingReplicas() override;

    private:

        std::unordered_map<boost::uuids::uuid, std::shared_ptr<dafs::Signal>> progress_map;

        paxos::Parliament parliament;

        std::mutex mutex;
    };
}
