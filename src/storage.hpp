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
    };


    class ReplicatedStorage : public Storage
    {
    public:

        void Update(Block was, Block is);

    private:

        Parliament parliament;
    };
}


#endif
