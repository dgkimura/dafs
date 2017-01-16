#pragma once

#include "dafs/filesystem.hpp"


namespace dafs
{
    class Loader
    {
    public:

        dafs::LocationIndex GetLocations(dafs::BlockInfo nodes);
    };
}
