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

        virtual void CreateFile(dafs::FileInfo info) = 0;

        virtual void DeleteFile(FileInfo file) = 0;

        virtual void CreateBlock(BlockInfo block) = 0;

        virtual void DeleteBlock(BlockInfo block) = 0;

        virtual BlockFormat ReadBlock(BlockInfo block) = 0;

        virtual void WriteBlock(BlockInfo block, BlockFormat format) = 0;

        virtual void AddNode(std::string address, short port) = 0;

        virtual void RemoveNode(std::string address, short port) = 0;
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

        virtual dafs::Identity GetIdentity();

        virtual void SetIdentity(dafs::Identity id);

        virtual void CreateFile(dafs::FileInfo info);

        virtual void DeleteFile(FileInfo file);

        virtual void CreateBlock(BlockInfo block);

        virtual void DeleteBlock(BlockInfo block);

        virtual BlockFormat ReadBlock(BlockInfo block);

        virtual void WriteBlock(BlockInfo block, BlockFormat format);

        virtual void AddNode(std::string address, short port);

        virtual void RemoveNode(std::string address, short port);

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
