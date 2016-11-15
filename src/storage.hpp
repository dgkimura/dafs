#pragma once

#include <string>

#include <paxos/parliament.hpp>

#include "filesystem.hpp"
#include "proposaltype.hpp"


namespace dafs
{
    class Storage
    {
    public:

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


    class ReplicatedStorage : public Storage
    {
    public:

        ReplicatedStorage(std::string directory);

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

    private:

        void do_write(dafs::ProposalType type, BlockInfo info, std::string data);

        Parliament parliament;

        dafs::BlockInfo file_info_list;

        dafs::BlockInfo block_info_list;
    };
}
