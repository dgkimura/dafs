#pragma once

#include <boost/filesystem.hpp>

#include <string>
#include <vector>


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
    // Describes location.
    //
    struct Location
    {
        std::string address;

        short port;
    };


    //
    // Describes the block info.
    //
    struct BlockInfo
    {
        std::string path;

        Location location;

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
        Location previous;

        Location next;

        int descriptor;

        std::string path;

        bool
        operator==(const FileInfo& rhs) const
        {
            return path == rhs.path;
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


    using LocationIndex = Index<Location>;


    //
    // Function to create Bytes
    //
    Bytes CreateBytes(const std::string& data);


    //
    // Function to create Location
    //
    Location CreateLocation(const std::string& address);


    //
    // Function to create BlockInfo
    //
    BlockInfo CreateBlockInfo(const std::string& path,
                              const dafs::Location location);
}
