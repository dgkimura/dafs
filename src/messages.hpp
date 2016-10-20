#pragma once

#include <string>
#include <vector>

#include "metadata.hpp"


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
        // Open a file for further operations.
        //
        OpenFile,

        //
        // Read from an opened file.
        //
        ReadFile,

        //
        // Write to an opened file.
        //
        WriteFile,

        //
        // [Internal] Read from a known block.
        //
        _ReadBlock,

        //
        // [Internal] Write to a known block.
        //
        _WriteBlock,

        //
        // [Internal] Allocate resources.
        //
        _Allocate,

        //
        // [Internal] Allocated resources.
        //
        _Allocated,
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
