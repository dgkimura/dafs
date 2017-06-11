#pragma once

#include <limits>
#include <sstream>
#include <string>
#include <vector>

#include "dafs/identity.hpp"


namespace dafs
{
    const int BLOCK_SIZE_IN_BYTES = 65536;

    const int BLOCKS_IN_FILEINFO = 8192;

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
    // Defines segment of bytes. (used to write to blocks)
    //
    struct Bytes
    {
        char contents[BLOCK_SIZE_IN_BYTES];
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


    //
    // Describes the file info.
    //
    struct FileInfo
    {
        Identity current;

        Identity previous;

        Identity next;

        std::string path;

        bool
        operator==(const FileInfo& rhs) const
        {
            return path == rhs.path && current == rhs.current;
        }
    };


    //
    // Defines the file structure on disk.
    //
    struct FileFormat
    {
        FileInfo info;

        BlockInfo blocks[BLOCKS_IN_FILEINFO];
    };


    template<typename T>
    struct Index
    {
        std::vector<T> items;
    };


    using FileIndex = Index<FileInfo>;


    using BlockIndex = Index<BlockInfo>;


    //
    // Function to create Bytes
    //
    Bytes CreateBytes(const std::string& data);


    bool IsLogicallyOrdered(dafs::Identity first,
                            dafs::Identity second,
                            dafs::Identity third);


    BlockIndex SplitUpperIndex(BlockIndex index,
                               dafs::Identity lower,
                               dafs::Identity divider,
                               dafs::Identity upper);


    BlockIndex SplitLowerIndex(BlockIndex index,
                               dafs::Identity lower,
                               dafs::Identity divider,
                               dafs::Identity upper);
}
