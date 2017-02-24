#pragma once

#include "dafs/filesystem.hpp"
#include "dafs/messages.hpp"


namespace dafs
{
    struct PartitionDetails
    {
        dafs::Address author;

        dafs::Address interface;

        dafs::Identity identity;
    };


    struct NodeDetails
    {
        dafs::PartitionDetails minus_details;

        dafs::PartitionDetails zero_details;

        dafs::PartitionDetails plus_details;
    };
}
