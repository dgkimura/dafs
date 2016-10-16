#pragma once

#include <string>

#include "paxos/parliament.hpp"

#include "delta.hpp"
#include "filesystem.hpp"


namespace dafs
{
    class Persister
    {
    public:

        Persister(Parliament parliament);

        BlockFormat Get(BlockInfo info);

        void Put(BlockInfo info, Delta delta);

    private:

        Parliament parliament;
    };


    class Storage
    {
    public:

        Storage(Persister persister);

        void CreateFile(FileInfo file);

        void DeleteFile(FileInfo file);

        void ReadFile(FileInfo file);

        void WriteFile(FileInfo info, Bytes data);

        void CreateBlock(BlockInfo block);

        void DeleteBlock(BlockInfo block);

        void ReadBlock(BlockInfo block);

        void WriteBlock(BlockInfo info, Bytes data);

    private:

        Persister persister;
    };
}
