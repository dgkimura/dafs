#ifndef __STORAGE_HPP_INCLUDED__
#define __STORAGE_HPP_INCLUDED__

#include <string>

#include "paxos/parliament.hpp"

#include "filesystem.hpp"


namespace dafs
{
    class Loader
    {
    public:

        Block Fetch(BlockInfo info);
    };


    class Persister
    {
    public:

        Persister(Parliament parliament);

        void Update(BlockInfo info, Block was, Block is);

    private:

        Parliament parliament;
    };


    class Storage
    {
    public:

        Storage(Loader loader, Persister persister);

        void Save(BlockInfo info, Block block);

        Block Fetch(BlockInfo info);

    private:

        Loader loader;

        Persister persister;
    };
}


#endif
