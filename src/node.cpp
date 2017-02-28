#include "dafs/node.hpp"

namespace dafs
{
    Node::Node(
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
    Node::GetPartition(Slot slot)
    {
        switch (slot)
        {
            case Slot::Minus:
            {
                return slot_minus;
            }
            case Slot::Zero:
            {
                return slot_zero;
            }
            case Slot::Plus:
            {
                return slot_plus;
            }
        }
    }


    std::shared_ptr<dafs::Partition>
    Node::GetPartition(Identity identity)
    {
        auto minus_id = slot_minus->GetDetails().identity;
        auto zero_id = slot_zero->GetDetails().identity;
        auto plus_id = slot_plus->GetDetails().identity;

        if (minus_id <= identity &&
            identity < minus_id.Median(zero_id) &&
            slot_minus->IsActive())
        {
            return slot_minus;
        }
        else if (identity >= minus_id.Median(zero_id) &&
                 identity < zero_id.Median(plus_id) &&
                 slot_zero->IsActive())
        {
            return slot_zero;
        }
        else if (identity < plus_id &&
                 identity >= minus_id.Median(zero_id) &&
                 slot_plus->IsActive())
        {
            return slot_plus;
        }
        return slot_empty;
    }
}
