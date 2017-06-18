#pragma once

#include <functional>
#include <string>

#include "dafs/blocks.hpp"
#include "dafs/constants.hpp"
#include "dafs/delta.hpp"
#include "dafs/disk.hpp"
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

        virtual BlockIndex GetIndex() = 0;

        virtual void WriteBlock(BlockInfo info, BlockFormat data) = 0;

        virtual void Write(BlockInfo info, Delta delta) = 0;
    };


    class ReplicatedStorage : public Storage
    {
    public:

        ReplicatedStorage(
            dafs::Replication& replication,
            dafs::Root root);

        ReplicatedStorage(
            const ReplicatedStorage& other);

        virtual BlockFormat ReadBlock(BlockInfo block) override;

        virtual void DeleteBlock(BlockInfo info) override;

        virtual void InsertIndex(BlockInfo info) override;

        virtual void RemoveIndex(BlockInfo info) override;

        virtual BlockIndex GetIndex() override;

        virtual void WriteBlock(BlockInfo info, BlockFormat data) override;

        virtual void Write(BlockInfo info, Delta delta) override;

    private:

        BlockInfo rooted(BlockInfo info);

        dafs::Replication& replication;

        dafs::Root root;

        std::string blocklist;
    };


    class NodeSet
    {
    public:

        virtual void RemoveNode(dafs::Address address) = 0;

        virtual dafs::ReplicatedEndpoints SetMinus(
            const dafs::Address management,
            const dafs::Address replication,
            const dafs::Identity identity,
            const std::string location,
            dafs::ReplicatedEndpoints details) = 0;

        virtual dafs::ReplicatedEndpoints SetZero(
            const dafs::Address management,
            const dafs::Address replication,
            const dafs::Identity identity,
            const std::string location,
            dafs::ReplicatedEndpoints details) = 0;

        virtual dafs::ReplicatedEndpoints SetPlus(
            const dafs::Address management,
            const dafs::Address replication,
            const dafs::Identity identity,
            const std::string location,
            dafs::ReplicatedEndpoints details) = 0;
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
            const dafs::Identity identity,
            const std::string location,
            dafs::ReplicatedEndpoints details) override;

        virtual dafs::ReplicatedEndpoints SetZero(
            const dafs::Address management,
            const dafs::Address replication,
            const dafs::Identity identity,
            const std::string location,
            dafs::ReplicatedEndpoints details) override;

        virtual dafs::ReplicatedEndpoints SetPlus(
            const dafs::Address management,
            const dafs::Address replication,
            const dafs::Identity identity,
            const std::string location,
            dafs::ReplicatedEndpoints details) override;

    private:

        dafs::Replication& replication_;
    };


    class Ping
    {
    public:

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
            std::function<bool(void)> is_partition_locked,
            std::chrono::seconds ping_interval);

        std::vector<dafs::Address>
        NonresponsiveMembers(int last_elections=10) override;

        void Start();

        void SendPing(std::shared_ptr<dafs::Sender> sender) override;

    private:

        dafs::Endpoint get_failover_endpoint(dafs::Address inactive);

        dafs::Replication& replication;

        dafs::Address address_;

        std::function<dafs::ReplicatedEndpoints(void)> get_endpoints;

        std::function<bool(void)> is_partition_locked;

        std::chrono::seconds ping_interval;

        bool should_continue;
    };


    class Lock
    {
    public:

        virtual bool Acquire() = 0;

        virtual bool IsLocked() = 0;

        virtual void Release() = 0;
    };


    class ReplicatedLock : public Lock
    {
    public:

        ReplicatedLock(
            dafs::Replication& replication,
            dafs::Address address,
            dafs::Root root);

        virtual bool Acquire() override;

        virtual bool IsLocked() override;

        virtual void Release() override;

    private:

        BlockInfo rooted(BlockInfo info);

        dafs::Replication& replication;

        dafs::Address address;

        dafs::Root root;
    };


    class BlockAllocator
    {
    public:

        BlockAllocator(
            std::function<dafs::BlockIndex(void)> get_index,
            std::function<void(dafs::BlockInfo)> insert_index,
            std::function<dafs::ReplicatedEndpoints(void)> get_endpoints);

        dafs::BlockInfo Allocate();

    private:

        std::function<dafs::BlockIndex(void)> get_index;

        std::function<void(dafs::BlockInfo)> insert_index;

        std::function<dafs::ReplicatedEndpoints(void)> get_endpoints;
    };
}

