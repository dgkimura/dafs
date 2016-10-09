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
        // Allocate resources.
        //
        Allocate,

        //
        // Allocated resources.
        //
        Allocated,

        //
        // Ping to neighbor node.
        //
        Ping
    };


    struct Message
    {
        std::string from;
        std::string to;
        std::string content;
        MessageType type;
        std::vector<MetaData> metadata;
    };
}
