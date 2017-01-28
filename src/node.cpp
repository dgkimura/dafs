#include "dafs/node.hpp"

namespace dafs
{
    Node::Node()
        : slot_minus(dafs::Root("p-minus")),
          slot_zero(dafs::Root("p-zero")),
          slot_plus(dafs::Root("p-plus"))
    {
    }


    dafs::Partition&
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


    dafs::Partition&
    Node::GetPartition(Identity identity)
    {
        if (identity < slot_minus.GetIdentity())
        {
            return slot_minus;
        }
        else if (slot_plus.GetIdentity() < identity)
        {
            return slot_plus;
        }
        else
        {
            return slot_zero;
        }
    }
}
