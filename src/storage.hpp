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

        void Set(BlockInfo info, Delta delta);

    private:

        Parliament parliament;
    };


    class Storage
    {
    public:

        Storage(Persister persister);

        BlockFormat Load(BlockInfo info);

        void Save(BlockInfo info, BlockFormat block);

        std::string Read(BlockInfo info, int offset, int bytes);

        void Write(BlockInfo info, int offset, std::string data);

    private:

        Persister persister;
    };
}
