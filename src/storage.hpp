#ifndef __STORAGE_HPP_INCLUDED__
#define __STORAGE_HPP_INCLUDED__

#include <string>

#include "paxos.hpp"

#include "filesystem.hpp"


namespace dafs
{
    class Storage
    {
    public:

        virtual void Update(Block was, Block is) = 0;

        virtual void Get(Block block) = 0;
    };


    class Loader
    {
    public:

        void Fetch(Block& block);
    };


    class ReplicatedStorage : public Storage
    {
    public:

        void Update(Block was, Block is);

        void Get(Block block);

    private:

        Parliament parliament;

        Loader loader;
    };
}


#endif
