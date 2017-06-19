#pragma once

#include <string>
#include <vector>

#include "dafs/identity.hpp"


namespace dafs
{
    const int BLOCK_SIZE_IN_BYTES = 16384;

    //
    // Defines directory where partition is located.
    //
    struct Root
    {
        std::string directory;

        Root(std::string directory)
            : directory(directory)
        {
        }
    };


    //
    // Describes the block info.
    //
    struct BlockInfo
    {
        std::string path;

        Identity identity;

        int revision;

        bool
        operator==(const BlockInfo& rhs) const
        {
            return path == rhs.path && revision == rhs.revision;
        }
    };


    //
    // Defines the block structure on disk.
    //
    struct BlockFormat
    {
        std::string contents;
    };


    template<typename T>
    struct Index
    {
        std::vector<T> items;
    };


    using BlockIndex = Index<BlockInfo>;


    BlockIndex SplitUpperIndex(BlockIndex index,
                               dafs::Identity lower,
                               dafs::Identity divider,
                               dafs::Identity upper);


    BlockIndex SplitLowerIndex(BlockIndex index,
                               dafs::Identity lower,
                               dafs::Identity divider,
                               dafs::Identity upper);
}
