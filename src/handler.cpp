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
        dafs::Message m;
        m.metadata = std::vector<dafs::MetaData>
        {
            dafs::MetaData
            {
                dafs::MinusReplicatedEndpointsKey,
                dafs::Serialize<dafs::ReplicatedEndpoints>(
                    node.GetPartition(dafs::Node::Slot::Minus)->GetNodeSetDetails()
                )
            },
            dafs::MetaData
            {
                dafs::MinusIdentityKey,
                dafs::Serialize<dafs::Identity>(
                    node.GetPartition(dafs::Node::Slot::Minus)->GetIdentity()
                )
            },
            dafs::MetaData
            {
                dafs::ZeroReplicatedEndpointsKey,
                dafs::Serialize<dafs::ReplicatedEndpoints>(
                    node.GetPartition(dafs::Node::Slot::Zero)->GetNodeSetDetails()
                )
            },
            dafs::MetaData
            {
                dafs::ZeroIdentityKey,
                dafs::Serialize<dafs::Identity>(
                    node.GetPartition(dafs::Node::Slot::Zero)->GetIdentity()
                )
            },
            dafs::MetaData
            {
                dafs::PlusReplicatedEndpointsKey,
                dafs::Serialize<dafs::ReplicatedEndpoints>(
                    node.GetPartition(dafs::Node::Slot::Plus)->GetNodeSetDetails()
                )
            },
            dafs::MetaData
            {
                dafs::PlusIdentityKey,
                dafs::Serialize<dafs::Identity>(
                    node.GetPartition(dafs::Node::Slot::Plus)->GetIdentity()
                )
            }
        };
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

                dafs::Endpoint previous = p_minus->GetNodeSetDetails().plus;

                // Push replication onto initiated node.
                p_minus->SetPlus(
                    endpoints.minus.management,
                    endpoints.minus.replication,
                    Constant::PartitionMinusName);

                endpoints.minus = previous;

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
                    endpoints.minus.management,
                    endpoints.minus.replication,
                    Constant::PartitionMinusName);
                p_zero->SetMinus(
                    endpoints.plus.management,
                    endpoints.plus.replication,
                    Constant::PartitionPlusName);
                endpoints.plus = p_minus->GetNodeSetDetails().plus;
                endpoints.minus = p_plus->GetNodeSetDetails().minus;
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

        auto p_minus = node.GetPartition(dafs::Node::Slot::Minus);
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

            dafs::Endpoint previous = p_plus->GetNodeSetDetails().minus;

            // Push replication onto initiated node.
            p_plus->SetMinus(
                endpoints.plus.management,
                endpoints.plus.replication,
                Constant::PartitionPlusName);

            // swap FIXME
            endpoints.plus = endpoints.minus;
            endpoints.minus = previous;

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


    dafs::Message
    HandleExitCluster(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        auto p_minus = node.GetPartition(dafs::Node::Slot::Minus);
        auto p_zero = node.GetPartition(dafs::Node::Slot::Zero);

        sender.Send(
            dafs::Message
            {
                p_zero->GetNodeSetDetails().zero.management,
                p_minus->GetNodeSetDetails().zero.management,
                dafs::MessageType::_RequestPlusExit,
                std::vector<dafs::MetaData>
                {
                    dafs::MetaData
                    {
                        dafs::IdentityKey,
                        dafs::Serialize(p_zero->GetIdentity())
                    }
                }
            }
        );

        dafs::Message m;
        return m;
    }


    dafs::Message
    HandleRequestPlusExit(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        auto identity = metadata.GetValue<dafs::Identity>(dafs::IdentityKey);

        auto p_minus = node.GetPartition(dafs::Node::Slot::Minus);
        auto p_zero = node.GetPartition(dafs::Node::Slot::Zero);
        auto p_plus = node.GetPartition(dafs::Node::Slot::Plus);

        if (p_plus->GetIdentity() == identity)
        {
            p_zero->SetPlus(
                p_plus->GetNodeSetDetails().plus.management,
                p_plus->GetNodeSetDetails().plus.replication,
                Constant::PartitionMinusName);

            // TODO: Send p_plus's blocklist for pruning later...

            sender.Send(
                dafs::Message
                {
                    p_zero->GetNodeSetDetails().zero.management,
                    p_plus->GetNodeSetDetails().plus.management,
                    dafs::MessageType::_RequestMinusExit,
                    std::vector<dafs::MetaData>
                    {
                        dafs::MetaData
                        {
                            dafs::EndpointKey,
                            dafs::Serialize(p_plus->GetNodeSetDetails().minus)
                        }
                    }
                }
            );
        }

        dafs::Message m;
        return m;
    }


    dafs::Message
    HandleRequestMinusExit(
        dafs::Node& node,
        dafs::MetaDataParser metadata)
    {
        auto p_minus = node.GetPartition(dafs::Node::Slot::Minus);
        auto p_zero = node.GetPartition(dafs::Node::Slot::Zero);
        auto p_plus = node.GetPartition(dafs::Node::Slot::Plus);

        auto endpoint = metadata.GetValue<dafs::Endpoint>(dafs::EndpointKey);

        // TODO: Use blocklist to add _relavent_ files to p_minus and p_zero

        p_zero->SetMinus(
            endpoint.management,
            endpoint.replication,
            Constant::PartitionPlusName);

        dafs::Message m;
        return m;
    }
}
