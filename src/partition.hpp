#pragma once

#include "filesystem.hpp"
#include "nodeset.hpp"
#include "storage.hpp"


namespace dafs
{
    class Partition
    {
    public:

        Partition(
            dafs::Storage& store,
            dafs::NodeSet& nodeset,
            dafs::BlockInfo files,
            dafs::BlockInfo blocks,
            dafs::BlockInfo nodes,
            dafs::BlockInfo identity
        );

        int GetIdentity();

        void SetIdentity(int number);

        void CreateFile(dafs::FileInfo info);

        void DeleteFile(FileInfo file);

        void CreateBlock(BlockInfo block);

        void DeleteBlock(BlockInfo block);

        BlockFormat ReadBlock(BlockInfo block);

        void WriteBlock(BlockInfo block, BlockFormat format);

        void AddNode(std::string address, short port);

        void RemoveNode(std::string address, short port);

    private:

        Storage& store;

        NodeSet& nodeset;

        dafs::BlockInfo files;

        dafs::BlockInfo blocks;

        dafs::BlockInfo nodes;

        dafs::BlockInfo identity;
    };
}
