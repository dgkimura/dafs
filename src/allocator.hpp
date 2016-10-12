#pragma once

#include <vector>

#include "filesystem.hpp"


namespace dafs
{
    struct Allocation
    {
        std::vector<dafs::BlockInfo> blocks;
    };
}
