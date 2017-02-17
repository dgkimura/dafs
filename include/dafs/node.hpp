#pragma once

#include "dafs/filesystem.hpp"
#include "dafs/messages.hpp"
#include "dafs/partition.hpp"


namespace dafs
{
    class Node
    {
    public:

        enum class Slot
        {
            Minus,
            Zero,
            Plus
        };

        Node(
            dafs::Address partition_minus_address,
            dafs::Address partition_zero_address,
            dafs::Address partition_plus_address
        );

        std::shared_ptr<dafs::Partition> GetPartition(Node::Slot slot);

        std::shared_ptr<dafs::Partition> GetPartition(dafs::Identity identity);

    private:

        std::shared_ptr<dafs::Partition> slot_minus;

        std::shared_ptr<dafs::Partition> slot_zero;

        std::shared_ptr<dafs::Partition> slot_plus;

        std::shared_ptr<dafs::Partition> slot_empty;
    };
}
