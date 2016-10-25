#pragma once

#include <string>

#include "paxos/parliament.hpp"

#include "delta.hpp"
#include "filesystem.hpp"


namespace dafs
{
    class Persistence
    {
    public:

        virtual BlockFormat Get(BlockInfo info) = 0;

        virtual void Put(BlockInfo info, Bytes data) = 0;
    };


    class Durable : public Persistence
    {
    public:

        Durable(Parliament parliament, std::string dirname);

        BlockFormat Get(BlockInfo info) override;

        void Put(BlockInfo info, Bytes data) override;

    private:

        Parliament parliament;

        std::string dirname;
    };


    //
    // Defines the parent block info.
    //
    const BlockInfo SuperBlock = dafs::CreateBlockInfo(
        "blocklist",
        dafs::CreateLocation("localhost")
    );


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
