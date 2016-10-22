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

        virtual void Put(BlockInfo info, Delta delta) = 0;
    };


    class Durable : public Persistence
    {
    public:

        Durable(Parliament parliament);

        BlockFormat Get(BlockInfo info) override;

        void Put(BlockInfo info, Delta delta) override;

    private:

        Parliament parliament;
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
