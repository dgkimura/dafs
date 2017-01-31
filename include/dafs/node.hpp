#pragma once

#include "dafs/filesystem.hpp"
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

        Node();

        dafs::Partition& GetPartition(Node::Slot slot);

        dafs::Partition& GetPartition(dafs::Identity identity);

    private:

        dafs::Partition slot_minus;

        dafs::Partition slot_zero;

        dafs::Partition slot_plus;

        dafs::Partition slot_empty;
    };
}
