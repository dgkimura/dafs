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

        //
        // Identity associated with the management endpoint.
        //
        dafs::Identity identity;
    };


    struct ReplicatedEndpoints
    {
        dafs::Endpoint minus;

        dafs::Endpoint zero;

        dafs::Endpoint plus;
    };


    dafs::Endpoint GetFailover(
        dafs::ReplicatedEndpoints endpoints,
        dafs::Address active,
        dafs::Address inactive);


    bool IsReplicatedPartition(
        dafs::ReplicatedEndpoints endpoints);
}
