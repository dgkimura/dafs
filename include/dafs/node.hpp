#pragma once

#include "dafs/messages.hpp"
#include "dafs/identity.hpp"
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

        virtual std::shared_ptr<dafs::Partition> GetPartition(
            Node::Slot slot) = 0;

        virtual std::shared_ptr<dafs::Partition> GetPartition(
            dafs::Identity identity) = 0;
    };


    class ReplicatedNode : public Node
    {
    public:

        ReplicatedNode(
            std::shared_ptr<dafs::Partition> pminus,
            std::shared_ptr<dafs::Partition> pzero,
            std::shared_ptr<dafs::Partition> pplus
        );

        virtual std::shared_ptr<dafs::Partition> GetPartition(Node::Slot slot) override;

        virtual std::shared_ptr<dafs::Partition> GetPartition(dafs::Identity identity) override;

    private:

        std::shared_ptr<dafs::Partition> slot_minus;

        std::shared_ptr<dafs::Partition> slot_zero;

        std::shared_ptr<dafs::Partition> slot_plus;

        std::shared_ptr<dafs::Partition> slot_empty;
    };
}
