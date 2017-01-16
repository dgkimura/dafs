#pragma once

#include <string>

#include <paxos/parliament.hpp>

#include "dafs/delta.hpp"
#include "dafs/disk.hpp"
#include "dafs/filesystem.hpp"
#include "dafs/propose.hpp"
#include "dafs/signal.hpp"
#include "dafs/serialization.hpp"


namespace
{
    struct Constant
    {
        static constexpr const char * FileListName = "filelist";

        static constexpr const char * BlockListName = "blocklist";

        static constexpr const char * NodeSetName = "nodeset";

        static constexpr const char * IdentityName = "identity";
    };
};


namespace dafs
{
    class Storage
    {
    public:

        virtual BlockFormat ReadBlock(BlockInfo block) = 0;

        virtual void WriteBlock(BlockInfo info, BlockFormat data) = 0;

        virtual void Write(BlockInfo info, Delta delta) = 0;
    };


    class ReplicatedStorage : public Storage
    {
    public:

        ReplicatedStorage(
            Parliament& parliament,
            dafs::Signal& in_progress);

        ReplicatedStorage(
            const ReplicatedStorage& other);

        virtual BlockFormat ReadBlock(BlockInfo block) override;

        virtual void WriteBlock(BlockInfo info, BlockFormat data) override;

        virtual void Write(BlockInfo info, Delta delta) override;

    private:

        virtual void do_write(
            BlockInfo info,
            std::string data);

        Parliament parliament;

        Signal& in_progress;
    };


    class NodeSet
    {
    public:

        virtual void AddNode(std::string address, short port) = 0;

        virtual void RemoveNode(std::string address, short port) = 0;
    };


    class ReplicatedNodeSet : public NodeSet
    {
    public:

        ReplicatedNodeSet(
            Parliament& parliament,
            dafs::Signal& in_progress);

        ReplicatedNodeSet(
            const ReplicatedNodeSet& other);

        virtual void AddNode(std::string address, short port) override;

        virtual void RemoveNode(std::string address, short port) override;

    private:

        Parliament& parliament;

        dafs::Signal& in_progress;
    };
}
