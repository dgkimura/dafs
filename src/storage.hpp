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

        Block Fetch(Block block);
    };


    class Persister
    {
    public:

        Persister(Parliament parliament);

        void Update(Block was, Block is);

    private:

        Parliament parliament;
    };


    class Storage
    {
    public:

        Storage(Loader loader, Persister persister);

        void Save(Block block);

        Block Fetch(Block block);

    private:

        Loader loader;

        Persister persister;
    };
}


#endif
