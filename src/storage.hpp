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

        void CreateBlock(BlockInfo block);

        void DeleteBlock(BlockInfo block);

        BlockFormat ReadBlock(BlockInfo block);

        void Write(BlockInfo info, Bytes data);

    private:

        Persister persister;
    };
}
