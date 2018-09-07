#include "dafs/node.hpp"

namespace dafs
{
    ReplicatedNode::ReplicatedNode(
        std::shared_ptr<dafs::Partition> pminus,
        std::shared_ptr<dafs::Partition> pzero,
        std::shared_ptr<dafs::Partition> pplus
    )
        : slot_minus(pminus),
          slot_zero(pzero),
          slot_plus(pplus),
          slot_empty(std::make_shared<dafs::EmptyPartition>(
              dafs::Identity("00000000-0000-0000-0000-000000000000")))
    {
    }


    std::shared_ptr<dafs::Partition>
    ReplicatedNode::GetPartition(Node::Slot slot)
    {
        switch (slot)
        {
            case Node::Slot::Minus:
            {
                return slot_minus;
            }
            case Node::Slot::Zero:
            {
                return slot_zero;
            }
            case Node::Slot::Plus:
            {
                return slot_plus;
            }
        }
    }


    std::shared_ptr<dafs::Partition>
    ReplicatedNode::GetPartition(Identity identity)
    {
        auto minus_detail = slot_minus->GetNodeSetDetails();
        auto zero_detail = slot_zero->GetNodeSetDetails();
        auto plus_detail = slot_plus->GetNodeSetDetails();

        if (minus_detail.zero.identity == identity &&
            slot_minus->IsActive())
        {
            return slot_minus;
        }
        else if (zero_detail.zero.identity == identity &&
                 slot_zero->IsActive())
        {
            return slot_zero;
        }
        else if (plus_detail.zero.identity == identity &&
                 slot_plus->IsActive())
        {
            return slot_plus;
        }
        else if (IsLogicallyBetween(zero_detail.zero.identity,
                                    identity,
                                    zero_detail.plus.identity) &&
                 slot_zero->IsActive())
        {
            return slot_zero;
        }
        else if (IsLogicallyBetween(minus_detail.zero.identity,
                                    identity,
                                    minus_detail.plus.identity) &&
            slot_minus->IsActive())
        {
            return slot_minus;
        }
        else if (IsLogicallyBetween(plus_detail.zero.identity,
                                    identity,
                                    plus_detail.plus.identity) &&
                 slot_plus->IsActive())
        {
            return slot_plus;
        }
        else if (!slot_zero->IsActive())
        {
            return slot_zero;
        }
        return slot_empty;
    }
}
