#include "dafs/handler.hpp"


namespace dafs
{
    dafs::Message
    HandleReadBlock(
        dafs::Node& node,
        dafs::MetaDataParser metadata)
    {
        auto blockinfo = metadata.GetValue<dafs::BlockInfo>(dafs::BlockInfoKey);
        auto blockformat = node.GetPartition(blockinfo.identity)->ReadBlock(blockinfo);

        dafs::Message m;
        m.type = dafs::MessageType::ReadBlock;
        m.metadata.push_back(
            dafs::MetaData
            {
                dafs::BlockFormatKey,
                dafs::Serialize<dafs::BlockFormat>(blockformat)
            }
        );
        return m;
    }


    dafs::Message
    HandleWriteBlock(
        dafs::Node& node,
        dafs::MetaDataParser metadata)
    {
        auto blockinfo = metadata.GetValue<dafs::BlockInfo>(dafs::BlockInfoKey);
        auto block = metadata.GetValue<dafs::BlockFormat>(dafs::BlockFormatKey);

        node.GetPartition(blockinfo.identity)->WriteBlock(blockinfo, block);
        dafs::Message m;
        return m;
    }


    dafs::Message
    HandleDeleteBlock(
        dafs::Node& node,
        dafs::MetaDataParser metadata)
    {
        auto blockinfo = metadata.GetValue<dafs::BlockInfo>(dafs::BlockInfoKey);

        node.GetPartition(blockinfo.identity)->DeleteBlock(blockinfo);
        dafs::Message m;
        return m;
    }


    dafs::Message
    HandleGetNodeDetails(
        dafs::Node& node,
        dafs::MetaDataParser metadata)
    {
        auto details = dafs::NodeDetails
        {
            node.GetPartition(dafs::Node::Slot::Minus)->GetDetails(),
            node.GetPartition(dafs::Node::Slot::Zero)->GetDetails(),
            node.GetPartition(dafs::Node::Slot::Plus)->GetDetails()
        };
        dafs::Message m;
        m.metadata.push_back(
            dafs::MetaData
            {
                dafs::NodeDetailsKey,
                dafs::Serialize<dafs::NodeDetails>(details)
            }
        );
        return m;
    }


    dafs::Message
    HandleJoinCluster(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        auto p_minus = node.GetPartition(dafs::Node::Slot::Minus);
        auto p_zero = node.GetPartition(dafs::Node::Slot::Zero);
        auto p_plus = node.GetPartition(dafs::Node::Slot::Plus);

        sender.Send(
            dafs::Message
            {
                node.GetPartition(dafs::Node::Slot::Zero)->GetNodeSetDetails().zero.management,
                metadata.GetValue<dafs::Address>(dafs::AddressKey),
                dafs::MessageType::_RequestMinusInitiation,
                std::vector<dafs::MetaData>
                {
                    dafs::MetaData
                    {
                        dafs::NodeEndpointsKey,
                        dafs::Serialize(
                            dafs::ReplicatedEndpoints
                            {
                                p_minus->GetNodeSetDetails().plus,
                                p_zero->GetNodeSetDetails().zero,
                                p_plus->GetNodeSetDetails().minus
                            }
                        )
                    },
                    dafs::MetaData
                    {
                        dafs::IdentityKey,
                        dafs::Serialize(
                            node.GetPartition(
                                dafs::Node::Slot::Zero
                            )->GetIdentity()
                        )
                    }
                }
            }
        );

        dafs::Message m;
        return m;
    }


    dafs::Message
    HandleRequestMinusInitiation(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        auto identity = metadata.GetValue<dafs::Identity>(dafs::IdentityKey);
        auto endpoints = metadata.GetValue<dafs::ReplicatedEndpoints>(
            dafs::NodeEndpointsKey);

        auto p_minus = node.GetPartition(dafs::Node::Slot::Minus);
        auto p_zero = node.GetPartition(dafs::Node::Slot::Zero);
        auto p_plus = node.GetPartition(dafs::Node::Slot::Plus);

        if (p_minus->IsActive() &&
            (!IsLogicallyOrdered(p_minus->GetIdentity(),
                                 identity,
                                 p_zero->GetIdentity())))
        {
            //
            // Reject initiation request by telling node who to ask next.
            //
            sender.Send(
                dafs::Message
                {
                    p_zero->GetNodeSetDetails().zero.management,
                    endpoints.zero.management,
                    dafs::MessageType::_JoinCluster,
                    std::vector<dafs::MetaData>
                    {
                        dafs::MetaData
                        {
                            dafs::AddressKey,
                            dafs::Serialize(p_minus->GetNodeSetDetails().zero.management)
                        }
                    }
                }
            );
        }
        else
        {
            if (p_minus->IsActive())
            {
                //
                // Accept initiation request and update half of topology.
                //

                // Push replication onto initiated node.
                p_minus->SetPlus(
                    endpoints.minus.management,
                    endpoints.minus.replication,
                    Constant::PartitionMinusName);

                // Delete extra replications.
                p_minus->RemoveNode(p_minus->GetDetails().interface);

                endpoints.minus.replication = p_minus->GetDetails().interface;

                sender.Send(
                    dafs::Message
                    {
                        p_zero->GetNodeSetDetails().zero.management,
                        p_minus->GetNodeSetDetails().zero.management,
                        dafs::MessageType::_RequestPlusInitiation,
                        std::vector<dafs::MetaData>
                        {
                            dafs::MetaData
                            {
                                dafs::NodeEndpointsKey,
                                dafs::Serialize(
                                    endpoints
                                )
                            },
                            dafs::MetaData
                            {
                                dafs::IdentityKey,
                                dafs::Serialize(
                                    identity
                                )
                            }
                        }
                    }
                );
            }
            else if (!p_minus->IsActive())
            {
                //
                // Case of standalone node - accept initiation request and
                // update both halves of topology.
                //
                p_zero->SetPlus(
                    endpoints.plus.management,
                    endpoints.plus.replication,
                    Constant::PartitionPlusName);
                p_zero->SetMinus(
                    endpoints.minus.management,
                    endpoints.minus.replication,
                    Constant::PartitionMinusName);
                endpoints.plus.replication = p_minus->GetDetails().interface;
                endpoints.minus.replication = p_plus->GetDetails().interface;
                sender.Send(
                    dafs::Message
                    {
                        p_zero->GetNodeSetDetails().zero.management,
                        endpoints.zero.management,
                        dafs::MessageType::_AcceptPlusInitiation,
                        std::vector<dafs::MetaData>
                        {
                            dafs::MetaData
                            {
                                dafs::NodeEndpointsKey,
                                dafs::Serialize(
                                    endpoints
                                )
                            }
                      }
                    }
                );
            }
        }

        dafs::Message m;
        return m;
    }


    dafs::Message
    HandleRequestPlusInitiation(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        auto identity = metadata.GetValue<dafs::Identity>(dafs::IdentityKey);
        auto endpoints = metadata.GetValue<dafs::ReplicatedEndpoints>(
            dafs::NodeEndpointsKey);

        auto p_minus = node.GetPartition(dafs::Node::Slot::Plus);
        auto p_zero = node.GetPartition(dafs::Node::Slot::Zero);
        auto p_plus = node.GetPartition(dafs::Node::Slot::Plus);

        if (!IsLogicallyOrdered(p_zero->GetIdentity(),
                                identity,
                                p_plus->GetIdentity()))
        {
            //
            // Foobar'd topology. Rollback?
            //
        }
        else
        {
            //
            // Update second half of topology
            //

            // Push replication onto initiated node.
            p_plus->SetMinus(
                endpoints.plus.management,
                endpoints.plus.replication,
                Constant::PartitionPlusName);

            // Delete extra replications.
            p_plus->RemoveNode(p_plus->GetDetails().interface);

            // swap
            endpoints.plus = endpoints.minus;
            endpoints.minus.replication = p_plus->GetDetails().interface;

            // Send accepted messge to node.
            sender.Send(
                dafs::Message
                {
                    p_zero->GetNodeSetDetails().zero.management,
                    endpoints.zero.management,
                    dafs::MessageType::_AcceptPlusInitiation,
                    std::vector<dafs::MetaData>
                    {
                        dafs::MetaData
                        {
                            dafs::NodeEndpointsKey,
                            dafs::Serialize(
                                endpoints
                            )
                        }
                    }
                }
            );
        }

        dafs::Message m;
        return m;
    }


    dafs::Message
    HandleAcceptPlusInitiation(
        dafs::Node& node,
        dafs::MetaDataParser metadata)
    {
        auto p_zero = node.GetPartition(dafs::Node::Slot::Zero);
        auto endpoints = metadata.GetValue<dafs::ReplicatedEndpoints>(
            dafs::NodeEndpointsKey);

        //
        // Push replication of new node.
        //
        p_zero->SetMinus(
            endpoints.minus.management,
            endpoints.minus.replication,
            Constant::PartitionPlusName);
        p_zero->SetPlus(
            endpoints.plus.management,
            endpoints.plus.replication,
            Constant::PartitionMinusName);

        dafs::Message m;
        return m;
    }
}
