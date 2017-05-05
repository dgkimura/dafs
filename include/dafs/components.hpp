#pragma once

#include <functional>
#include <string>

#include "dafs/constants.hpp"
#include "dafs/delta.hpp"
#include "dafs/disk.hpp"
#include "dafs/filesystem.hpp"
#include "dafs/messages.hpp"
#include "dafs/propose.hpp"
#include "dafs/replication.hpp"
#include "dafs/sender.hpp"
#include "dafs/signal.hpp"
#include "dafs/serialization.hpp"


namespace dafs
{
    class Storage
    {
    public:

        virtual BlockFormat ReadBlock(BlockInfo block) = 0;

        virtual void DeleteBlock(BlockInfo info) = 0;

        virtual void InsertIndex(BlockInfo info) = 0;

        virtual void RemoveIndex(BlockInfo info) = 0;

        virtual bool ContainsIndex(BlockInfo info) = 0;

        virtual void WriteBlock(BlockInfo info, BlockFormat data) = 0;

        virtual void Write(BlockInfo info, Delta delta) = 0;
    };


    class ReplicatedStorage : public Storage
    {
    public:

        ReplicatedStorage(
            dafs::Replication& replication,
            dafs::Root root,
            std::shared_ptr<dafs::Signal> in_progress);

        ReplicatedStorage(
            const ReplicatedStorage& other);

        virtual BlockFormat ReadBlock(BlockInfo block) override;

        virtual void DeleteBlock(BlockInfo info) override;

        virtual void InsertIndex(BlockInfo info) override;

        virtual void RemoveIndex(BlockInfo info) override;

        virtual bool ContainsIndex(BlockInfo info) override;

        virtual void WriteBlock(BlockInfo info, BlockFormat data) override;

        virtual void Write(BlockInfo info, Delta delta) override;

    private:

        virtual BlockInfo rooted(BlockInfo info);

        virtual void do_write(
            BlockInfo info,
            std::string data);

        dafs::Replication& replication;

        dafs::Root root;

        std::shared_ptr<Signal> in_progress;

        dafs::BlockInfo blocks;
    };


    class NodeSet
    {
    public:

        virtual void RemoveNode(dafs::Address address) = 0;

        virtual dafs::ReplicatedEndpoints SetMinus(
            const dafs::Address management,
            const dafs::Address replication,
            const std::string location,
            dafs::ReplicatedEndpoints& details) = 0;

        virtual dafs::ReplicatedEndpoints SetZero(
            const dafs::Address management,
            const dafs::Address replication,
            const std::string location,
            dafs::ReplicatedEndpoints& details) = 0;

        virtual dafs::ReplicatedEndpoints SetPlus(
            const dafs::Address management,
            const dafs::Address replication,
            const std::string location,
            dafs::ReplicatedEndpoints& details) = 0;
    };


    class ReplicatedNodeSet : public NodeSet
    {
    public:

        ReplicatedNodeSet(
            dafs::Replication& replication);

        virtual void RemoveNode(dafs::Address address) override;

        virtual dafs::ReplicatedEndpoints SetMinus(
            const dafs::Address management,
            const dafs::Address replication,
            const std::string location,
            dafs::ReplicatedEndpoints& details) override;

        virtual dafs::ReplicatedEndpoints SetZero(
            const dafs::Address management,
            const dafs::Address replication,
            const std::string location,
            dafs::ReplicatedEndpoints& details) override;

        virtual dafs::ReplicatedEndpoints SetPlus(
            const dafs::Address management,
            const dafs::Address replication,
            const std::string location,
            dafs::ReplicatedEndpoints& details) override;

    private:

        dafs::Replication& replication_;
    };


    class Ping
    {
        virtual std::vector<dafs::Address> NonresponsiveMembers(
            int last_elections=10) = 0;

        virtual void SendPing(std::shared_ptr<dafs::Sender> sender) = 0;
    };


    class ReplicatedPing : public Ping
    {
    public:

        ReplicatedPing(
            dafs::Replication& replication,
            dafs::Address address_,
            std::function<dafs::ReplicatedEndpoints(void)> get_endpoints,
            std::chrono::seconds ping_interval,
            std::shared_ptr<dafs::Signal> in_progress);

        std::vector<dafs::Address>
        NonresponsiveMembers(int last_elections=10) override;

        void Start();

        void SendPing(std::shared_ptr<dafs::Sender> sender) override;

    private:

        dafs::Endpoint get_failover_endpoint(dafs::Address address);

        dafs::Replication& replication;

        dafs::Address address_;

        std::function<dafs::ReplicatedEndpoints(void)> get_endpoints;

        std::chrono::seconds ping_interval;

        std::shared_ptr<Signal> in_progress;

        bool should_continue;
    };
}

