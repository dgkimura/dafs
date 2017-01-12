#pragma once

#include <paxos/parliament.hpp>

#include "filesystem.hpp"
#include "signal.hpp"
#include "replicated.hpp"


namespace dafs
{
    class Partition
    {
    public:

        Partition(
            Root root
        );

        Partition(
            const Partition& other
        );

        struct Identity
        {
            std::string identity;
        };

        Identity GetIdentity();

        void SetIdentity(Identity id);

        void CreateFile(dafs::FileInfo info);

        void DeleteFile(FileInfo file);

        void CreateBlock(BlockInfo block);

        void DeleteBlock(BlockInfo block);

        BlockFormat ReadBlock(BlockInfo block);

        void WriteBlock(BlockInfo block, BlockFormat format);

        void AddNode(std::string address, short port);

        void RemoveNode(std::string address, short port);

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
