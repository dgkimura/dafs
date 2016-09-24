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

        BlockFormat Fetch(BlockInfo info);
    };


    class Persister
    {
    public:

        Persister(Parliament parliament);

        void Update(BlockInfo info, BlockFormat was, BlockFormat is);

    private:

        Parliament parliament;
    };


    class Storage
    {
    public:

        Storage(Loader loader, Persister persister);

        void Save(BlockInfo info, BlockFormat block);

        BlockFormat Fetch(BlockInfo info);

    private:

        Loader loader;

        Persister persister;
    };
}


#endif
