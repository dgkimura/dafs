#pragma once

#include <string>

#include <paxos/parliament.hpp>

#include "dafs/constants.hpp"
#include "dafs/delta.hpp"
#include "dafs/disk.hpp"
#include "dafs/filesystem.hpp"
#include "dafs/messages.hpp"
#include "dafs/propose.hpp"
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
            Parliament& parliament,
            dafs::Root root,
            dafs::Signal& in_progress);

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

        Parliament parliament;

        dafs::Root root;

        Signal& in_progress;

        dafs::BlockInfo blocks;

        dafs::BlockInfo nodes;
    };


    class NodeSet
    {
    public:

        virtual void AddNode(dafs::Address address, std::string location) = 0;

        virtual void RemoveNode(dafs::Address address) = 0;
    };


    class ReplicatedNodeSet : public NodeSet
    {
    public:

        ReplicatedNodeSet(
            Parliament& parliament);

        virtual void AddNode(dafs::Address address, std::string location) override;

        virtual void RemoveNode(dafs::Address address) override;

    private:

        Parliament& parliament;
    };
}

