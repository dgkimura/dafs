#pragma once

#include <paxos/parliament.hpp>

#include "dafs/filesystem.hpp"
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

        virtual BlockFormat ReadBlock(BlockInfo block) = 0;

        virtual void WriteBlock(BlockInfo block, BlockFormat format) = 0;

        virtual void AddNode(std::string address, short port) = 0;

        virtual void RemoveNode(std::string address, short port) = 0;
    };


    class EmptyPartition : public Partition
    {
    public:

        virtual dafs::Identity GetIdentity() override
        {
            return dafs::Deserialize<dafs::Identity>("00000000-0000-0000-0000-000000000000");
        }

        virtual void SetIdentity(dafs::Identity id) override
        {
        }

        virtual void CreateBlock(BlockInfo block) override
        {
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

        virtual void AddNode(std::string address, short port) override
        {
        }

        virtual void RemoveNode(std::string address, short port) override
        {
        }
    };


    class ReplicatedPartition : public Partition
    {
    public:

        ReplicatedPartition(
            Root root
        );

        ReplicatedPartition(
            const ReplicatedPartition& other
        );

        virtual dafs::Identity GetIdentity() override;

        virtual void SetIdentity(dafs::Identity id) override;

        virtual void CreateBlock(BlockInfo block) override;

        virtual void DeleteBlock(BlockInfo block) override;

        virtual BlockFormat ReadBlock(BlockInfo block) override;

        virtual void WriteBlock(BlockInfo block, BlockFormat format) override;

        virtual void AddNode(std::string address, short port) override;

        virtual void RemoveNode(std::string address, short port) override;

    private:

        Parliament parliament;

        ReplicatedStorage store;

        ReplicatedNodeSet nodeset;

        dafs::BlockInfo files;

        dafs::BlockInfo blocks;

        dafs::BlockInfo nodes;

        dafs::BlockInfo identity;

        dafs::Signal in_progress;

        dafs::Root root;

        dafs::BlockInfo rooted(dafs::BlockInfo info);
    };
}
