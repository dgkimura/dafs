#pragma once

#include "dafs/filesystem.hpp"
#include "dafs/messages.hpp"


namespace dafs
{
    struct PartitionDetails
    {
        //
        // Management commection to partition owning node.
        //
        dafs::Address author;

        //
        // Replicated connection of this node for 2PC communication.
        //
        dafs::Address interface;

        //
        // Unique identity of the partition
        //
        dafs::Identity identity;
    };


    struct NodeDetails
    {
        dafs::PartitionDetails minus_details;

        dafs::PartitionDetails zero_details;

        dafs::PartitionDetails plus_details;
    };
}
