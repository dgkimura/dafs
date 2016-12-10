#pragma once

#include "filesystem.hpp"


namespace dafs
{
    class Loader
    {
    public:

        dafs::LocationIndex GetLocations(dafs::BlockInfo nodes);
    };
}
