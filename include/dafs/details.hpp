#pragma once

#include "dafs/filesystem.hpp"
#include "dafs/messages.hpp"


namespace dafs
{
    struct Endpoint
    {
        //
        // Endpoint used to communicate management operations to perform.
        //
        dafs::Address management;

        //
        // Endpoint used in underlying replication protocol.
        //
        dafs::Address replication;
    };


    struct ReplicatedEndpoints
    {
        dafs::Endpoint minus;

        dafs::Endpoint zero;

        dafs::Endpoint plus;
    };


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
}
