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

        virtual dafs::Identity GetIdentity() = 0;

        virtual void SetIdentity(dafs::Identity id) = 0;

        virtual void CreateBlock(BlockInfo block) = 0;

        virtual void DeleteBlock(BlockInfo block) = 0;

        virtual bool ContainsBlock(BlockInfo block) = 0;

        virtual BlockFormat ReadBlock(BlockInfo block) = 0;

        virtual void WriteBlock(BlockInfo block, BlockFormat format) = 0;

        virtual void AddNode(dafs::Address address) = 0;

        virtual void RemoveNode(dafs::Address address) = 0;
    };


    class EmptyPartition : public Partition
    {
    public:

        EmptyPartition(dafs::Identity identity)
            : identity(identity)
        {
        }

        virtual dafs::Identity GetIdentity() override
        {
            return identity;
        }

        virtual void SetIdentity(dafs::Identity id) override
        {
            identity = id;
        }

        virtual void CreateBlock(BlockInfo block) override
        {
        }

        virtual void DeleteBlock(BlockInfo block) override
        {
        }

        virtual bool ContainsBlock(BlockInfo block) override
        {
            return true;
        }

        virtual BlockFormat ReadBlock(BlockInfo block) override
        {
            return dafs::BlockFormat{""};
        }

        virtual void WriteBlock(BlockInfo block, BlockFormat format) override
        {
        }

        virtual void AddNode(dafs::Address address) override
        {
        }

        virtual void RemoveNode(dafs::Address address) override
        {
        }

    private:

        dafs::Identity identity;
    };


    class ReplicatedPartition : public Partition
    {
    public:

        ReplicatedPartition(
            Address address,
            Root root
        );

        ReplicatedPartition(
            const ReplicatedPartition& other
        );

        virtual dafs::Identity GetIdentity() override;

        virtual void SetIdentity(dafs::Identity id) override;

        virtual void CreateBlock(BlockInfo block) override;

        virtual void DeleteBlock(BlockInfo block) override;

        virtual bool ContainsBlock(BlockInfo block) override;

        virtual BlockFormat ReadBlock(BlockInfo block) override;

        virtual void WriteBlock(BlockInfo block, BlockFormat format) override;

        virtual void AddNode(dafs::Address address) override;

        virtual void RemoveNode(dafs::Address address) override;

    private:

        Parliament parliament;

        ReplicatedStorage store;

        ReplicatedNodeSet nodeset;

        dafs::BlockInfo blocks;

        dafs::BlockInfo nodes;

        dafs::BlockInfo identity;

        dafs::Signal in_progress;

        dafs::Root root;

        dafs::BlockInfo rooted(dafs::BlockInfo info);
    };
}
