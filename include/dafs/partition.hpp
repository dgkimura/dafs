#pragma once

#include <paxos/parliament.hpp>

#include "dafs/filesystem.hpp"
#include "dafs/messages.hpp"
#include "dafs/signal.hpp"
#include "dafs/replicated.hpp"


namespace dafs
{
    class Partition
    {
    public:

        virtual dafs::PartitionDetails GetDetails() = 0;

        virtual dafs::ReplicatedEndpoints GetNodeSetDetails() = 0;

        virtual void DeleteBlock(BlockInfo block) = 0;

        virtual BlockFormat ReadBlock(BlockInfo block) = 0;

        virtual void WriteBlock(BlockInfo block, BlockFormat format) = 0;

        virtual void AddNode(dafs::Address address, std::string location) = 0;

        virtual void RemoveNode(dafs::Address address) = 0;

        virtual void SetMinus(
            dafs::Address management,
            dafs::Address replication,
            std::string location) = 0;

        virtual void SetZero(
            dafs::Address management,
            dafs::Address replication,
            std::string location) = 0;

        virtual void SetPlus(
            dafs::Address management,
            dafs::Address replication,
            std::string location) = 0;

        virtual std::vector<dafs::Address> NonresponsiveMembers(
            int minimum_elections
        ) = 0;

        virtual bool IsActive() = 0;
    };


    class EmptyPartition : public Partition
    {
    public:

        EmptyPartition(dafs::Identity identity)
            : identity(identity)
        {
        }

        virtual dafs::PartitionDetails GetDetails() override
        {
            return dafs::PartitionDetails
            {
                dafs::Address("127.0.0.1", 9000),
                dafs::Address("127.0.0.1", 8080),
                identity
            };
        }

        virtual dafs::ReplicatedEndpoints GetNodeSetDetails() override
        {
            return dafs::ReplicatedEndpoints
            {
            };
        }

        virtual void DeleteBlock(BlockInfo block) override
        {
        }

        virtual BlockFormat ReadBlock(BlockInfo block) override
        {
            return dafs::BlockFormat{""};
        }

        virtual void WriteBlock(BlockInfo block, BlockFormat format) override
        {
        }

        virtual void AddNode(dafs::Address address, std::string location) override
        {
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
            return false;
        }

    private:

        dafs::Identity identity;
    };


    class ReplicatedPartition : public Partition
    {
    public:

        ReplicatedPartition(
            Address address,
            Root root,
            std::chrono::seconds ping_interval
        );

        ReplicatedPartition(
            const ReplicatedPartition& other
        );

        virtual dafs::PartitionDetails GetDetails() override;

        virtual dafs::ReplicatedEndpoints GetNodeSetDetails() override;

        virtual void DeleteBlock(BlockInfo block) override;

        virtual BlockFormat ReadBlock(BlockInfo block) override;

        virtual void WriteBlock(BlockInfo block, BlockFormat format) override;

        virtual void AddNode(dafs::Address address, std::string location) override;

        virtual void SetMinus(
            dafs::Address management,
            dafs::Address replication,
            std::string location) override;

        virtual void SetZero(
            dafs::Address management,
            dafs::Address replication,
            std::string location) override;

        virtual void SetPlus(
            dafs::Address management,
            dafs::Address replication,
            std::string location) override;

        virtual void RemoveNode(dafs::Address address) override;

        virtual std::vector<dafs::Address> NonresponsiveMembers(
            int minimum_elections
        ) override;

        virtual bool IsActive() override;

    private:

        Parliament parliament;

        ReplicatedStorage store;

        ReplicatedNodeSet nodeset;

        ReplicatedPing ping;

        dafs::BlockInfo identity;

        dafs::BlockInfo author;

        dafs::BlockInfo details;

        dafs::Signal in_progress;

        dafs::Address replication_interface;
    };
}
