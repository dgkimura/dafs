#include "dafs/node.hpp"

namespace dafs
{
    Node::Node()
        : slot_minus(dafs::Root("p-minus")),
          slot_zero(dafs::Root("p-zero")),
          slot_plus(dafs::Root("p-plus")),
          slot_empty(dafs::Root("p-empty"))
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
        if (slot_minus.GetIdentity() <= identity &&
            identity < slot_minus.GetIdentity().Median(slot_zero.GetIdentity()))
        {
            return slot_minus;
        }
        else if (identity >= slot_minus.GetIdentity().Median(slot_zero.GetIdentity()) &&
                 identity < slot_zero.GetIdentity().Median(slot_plus.GetIdentity()))
        {
            return slot_zero;
        }
        else if (identity < slot_plus.GetIdentity() &&
                 identity >= slot_minus.GetIdentity().Median(slot_zero.GetIdentity()))
        {
            return slot_plus;
        }
        return slot_empty;
    }
}
