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
        // Allocate resources.
        //
        _Allocate,

        //
        // Allocated resources.
        //
        _Allocated,

        //
        // Request initation - message sent to a node to signal it to request
        // an initiation into a given cluster.
        //
        _RequestInitiation,

        //
        // Processes initiation - message sent to node in a cluster to signal
        // that an outside node is attempting to initiate itself into this
        // cluster.
        //
        _ProcessInitation,

        //
        // Conclude initation - message is sent to node to signal that the
        // initation process has finished and provide the result of the
        // initation request.
        //
        _ConcludeInitation,
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
