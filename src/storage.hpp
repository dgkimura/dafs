#ifndef __STORAGE_HPP_INCLUDED__
#define __STORAGE_HPP_INCLUDED__

#include <string>

#include "paxos/parliament.hpp"

#include "delta.hpp"
#include "filesystem.hpp"


namespace dafs
{
    class Loader
    {
    public:

        BlockFormat Fetch(BlockInfo info);
    };


    class Persister
    {
    public:

        Persister(Parliament parliament);

        void Update(BlockInfo info, Delta delta);

    private:

        Parliament parliament;
    };


    class Storage
    {
    public:

        Storage(Loader loader, Persister persister);

        BlockFormat Load(BlockInfo info);

        void Save(BlockInfo info, BlockFormat block);

        std::string Read(BlockInfo info, int offset, int bytes);

        void Write(BlockInfo info, int offset, std::string data);

    private:

        Loader loader;

        Persister persister;
    };
}


#endif
