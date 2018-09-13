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


    std::vector<dafs::Endpoint>
    GetFaultDomainViolationEndpoints(dafs::Node& node)
    {
        std::vector<dafs::Endpoint> endpoints;

        auto p_minus = node.GetPartition(dafs::Node::Slot::Minus)->GetNodeSetDetails();
        auto p_zero = node.GetPartition(dafs::Node::Slot::Zero)->GetNodeSetDetails();
        auto p_plus = node.GetPartition(dafs::Node::Slot::Plus)->GetNodeSetDetails();

        if (p_minus.zero.identity == p_plus.zero.identity)
        {
            //
            // If there are only 2 nodes in the system then it is technically a
            // fault domain violation, but there is no action that can be taken
            // besides add another node.
            //
            return endpoints;
        }

        //        -p      -p 0p   -p 0p +p   0p +p      +p
        // [__,__,--] [__,-0,+-] [-+,00,+-] [0+,+0,__] [++,__,__]
        //   node-1     node-2     node-3     node-4     node-5
        //
        if (p_zero.zero.fault_domain == p_plus.plus.fault_domain)
        {
            endpoints.push_back(p_plus.plus);
        }
        if (p_minus.zero.fault_domain == p_plus.zero.fault_domain)
        {
            endpoints.push_back(p_plus.zero);
        }
        if (p_minus.minus.fault_domain == p_zero.zero.fault_domain)
        {
            endpoints.push_back(p_zero.zero);
        }
        return endpoints;
    }
}
