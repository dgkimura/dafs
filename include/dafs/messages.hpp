#pragma once

#include <string>
#include <vector>

#include "dafs/metadata.hpp"


namespace dafs
{
    enum class MessageType
    {
        //
        // Create a file inside the distributed system.
        //
        CreateFile,

        //
        // Delete a file from the distributed system.
        //
        DeleteFile,

        //
        // Read from a known block.
        //
        ReadBlock,

        //
        // Write to a known block.
        //
        WriteBlock,

        //
        // [Internal] Allocate resources.
        //
        _Allocate,

        //
        // [Internal] Allocated resources.
        //
        _Allocated,

        //
        // [Internal] Initiate node into a cluster.
        //
        _InitiateCluster,
    };


    struct Message
    {
        std::string from;
        std::string to;
        std::string content;
        dafs::MessageType type;
        std::vector<dafs::MetaData> metadata;
    };
}
