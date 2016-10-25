#pragma once

#include <string>

#include "paxos/parliament.hpp"

#include "delta.hpp"
#include "filesystem.hpp"
#include "proposals.hpp"


namespace dafs
{
    class Persistence
    {
    public:

        virtual BlockFormat Get(BlockInfo info) = 0;

        virtual void Write(BlockInfo info, Bytes data) = 0;

        virtual void Insert(BlockInfo info, FileInfo file) = 0;

        virtual void Remove(BlockInfo info, FileInfo file) = 0;
    };


    class Durable : public Persistence
    {
    public:

        Durable(Parliament parliament, std::string dirname);

        BlockFormat Get(BlockInfo info) override;

        void Write(BlockInfo info, Bytes data) override;

        void Insert(BlockInfo info, FileInfo file) override;

        void Remove(BlockInfo info, FileInfo file) override;

    private:

        Parliament parliament;

        std::string dirname;

        void do_write(dafs::ProposalType type, BlockInfo info, std::string);
    };


    class Storage
    {
    public:

        Storage(std::shared_ptr<Persistence> persister);

        void CreateFile(FileInfo file);

        void DeleteFile(FileInfo file);

        void OpenFile(FileInfo file);

        void ReadFile(FileInfo file);

        void WriteFile(FileInfo info, Bytes data);

        void CreateBlock(BlockInfo block);

        void DeleteBlock(BlockInfo block);

        void ReadBlock(BlockInfo block);

        void WriteBlock(BlockInfo info, Bytes data);

    private:

        std::shared_ptr<Persistence> persister;
    };
}
