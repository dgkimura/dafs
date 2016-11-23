#pragma once

#include <string>

#include <paxos/parliament.hpp>

#include "filesystem.hpp"
#include "proposaltype.hpp"


namespace
{
    struct Constant
    {
        static constexpr const char * FileListName = "filelist";

        static constexpr const char * BlockListName = "blocklist";

        static constexpr const char * NodeSetName = "nodeset";

        static constexpr const char * IdentityName = "identity";

        static constexpr const int UnknownId = -1;
    };
};


namespace dafs
{
    class Storage
    {
    public:

        virtual int GetIdentity() = 0;

        virtual void CreateFile(FileInfo file) = 0;

        virtual void DeleteFile(FileInfo file) = 0;

        virtual void OpenFile(FileInfo file) = 0;

        virtual FileFormat ReadFile(FileInfo file) = 0;

        virtual void WriteFile(FileInfo info, Bytes data) = 0;

        virtual void CreateBlock(BlockInfo block) = 0;

        virtual void DeleteBlock(BlockInfo block) = 0;

        virtual BlockFormat ReadBlock(BlockInfo block) = 0;

        virtual void WriteBlock(BlockInfo info, Bytes data) = 0;

        virtual void WriteBlock(BlockInfo info, BlockFormat data) = 0;
    };


    struct NodeSet
    {
        std::vector<std::string> endpoints;
    };


    class ReplicatedStorage : public Storage
    {
    public:

        ReplicatedStorage(
            std::string directory,
            std::string hostport,
            int identity=Constant::UnknownId);

        virtual int GetIdentity() override;

        virtual void CreateFile(FileInfo file) override;

        virtual void DeleteFile(FileInfo file) override;

        virtual void OpenFile(FileInfo file) override;

        virtual FileFormat ReadFile(FileInfo file) override;

        virtual void WriteFile(FileInfo info, Bytes data) override;

        virtual void CreateBlock(BlockInfo block) override;

        virtual void DeleteBlock(BlockInfo block) override;

        virtual BlockFormat ReadBlock(BlockInfo block) override;

        virtual void WriteBlock(BlockInfo info, Bytes data) override;

        virtual void WriteBlock(BlockInfo info, BlockFormat data) override;

        void AddNode(std::string address, short port);

        void RemoveNode(std::string address, short port);

    private:

        void do_write(
            dafs::ProposalType type,
            BlockInfo info,
            std::string data);

        void load_nodes(std::string hostport);

        void set_identity(int id);

        Parliament parliament;

        dafs::BlockInfo files_info;

        dafs::BlockInfo blocks_info;

        dafs::BlockInfo nodeset_info;

        dafs::BlockInfo identity_info;
    };
}
