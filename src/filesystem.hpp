#pragma once

#include <string>
#include <vector>


namespace dafs
{
    const int BLOCK_SIZE_IN_BYTES = 65536;

    const int BLOCKS_IN_FILEINFO = 8192;


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
    };


    //
    // Describes the block info.
    //
    struct BlockInfo
    {
        std::string filename;

        Location location;

        int offset;
    };


    //
    // Defines the block structure on disk.
    //
    struct BlockFormat
    {
        char contents[BLOCK_SIZE_IN_BYTES];
    };


    //
    // Describes the file info.
    //
    struct FileInfo
    {
        Location previous;

        Location next;

        int descriptor;

        std::string name;
    };


    //
    // Defines the file structure on disk.
    //
    struct FileFormat
    {
        FileInfo info;

        BlockInfo blocks[BLOCKS_IN_FILEINFO];
    };


    //
    // Defines the index structure on disk.
    //
    struct Index
    {
        std::vector<FileInfo> files;
    };


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
    BlockInfo CreateBlockInfo(const std::string& filename,
                              const dafs::Location location);
}
