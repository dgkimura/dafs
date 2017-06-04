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
                dafs::MessageType::_RequestJoinCluster,
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
    HandleRequestJoinCluster(
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
                if (!p_minus->Acquire() || !p_zero->Acquire())
                {
                    //
                    // Did not acquire both partition locks.
                    //
                    dafs::Message m;
                    return m;
                }

                //
                // Accept initiation request and update half of topology.
                //

                dafs::ReplicatedEndpoints updated;
                updated.plus = p_minus->GetNodeSetDetails().plus;
                updated.minus = p_zero->GetNodeSetDetails().minus;

                // Push replication onto initiated node.
                p_minus->SetPlus(
                    endpoints.minus.management,
                    endpoints.minus.replication,
                    endpoints.minus.identity,
                    Constant::PartitionMinusName);

                p_zero->SetMinus(
                    endpoints.plus.management,
                    endpoints.plus.replication,
                    endpoints.plus.identity,
                    Constant::PartitionPlusName);

                // Send accepted messge to node.
                sender.Send(
                    dafs::Message
                    {
                        p_zero->GetNodeSetDetails().zero.management,
                        endpoints.zero.management,
                        dafs::MessageType::_AcceptJoinCluster,
                        std::vector<dafs::MetaData>
                        {
                            dafs::MetaData
                            {
                                dafs::NodeEndpointsKey,
                                dafs::Serialize(
                                    updated
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
                    endpoints.plus.identity,
                    Constant::PartitionMinusName);
                p_zero->SetMinus(
                    endpoints.plus.management,
                    endpoints.plus.replication,
                    endpoints.plus.identity,
                    Constant::PartitionPlusName);
                endpoints.plus = p_minus->GetNodeSetDetails().plus;
                endpoints.minus = p_plus->GetNodeSetDetails().minus;
                sender.Send(
                    dafs::Message
                    {
                        p_zero->GetNodeSetDetails().zero.management,
                        endpoints.zero.management,
                        dafs::MessageType::_AcceptJoinCluster,
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
    HandleAcceptJoinCluster(
        dafs::Node& node,
        dafs::MetaDataParser metadata)
    {
        auto p_minus = node.GetPartition(dafs::Node::Slot::Minus);
        auto p_zero = node.GetPartition(dafs::Node::Slot::Zero);
        auto p_plus = node.GetPartition(dafs::Node::Slot::Plus);
        auto endpoints = metadata.GetValue<dafs::ReplicatedEndpoints>(
            dafs::NodeEndpointsKey);

        //
        // Push replication of new node.
        //
        p_zero->SetMinus(
            endpoints.minus.management,
            endpoints.minus.replication,
            endpoints.minus.identity,
            Constant::PartitionPlusName);
        p_zero->SetPlus(
            endpoints.plus.management,
            endpoints.plus.replication,
            endpoints.plus.identity,
            Constant::PartitionMinusName);

        p_minus->Release();
        p_zero->Release();
        p_plus->Release();

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
                dafs::MessageType::_PlusExitCluster,
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
    HandleProposeExitCluster(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        auto address = metadata.GetValue<dafs::Address>(dafs::AddressKey);

        auto p_minus = node.GetPartition(dafs::Node::Slot::Minus);
        auto p_zero = node.GetPartition(dafs::Node::Slot::Zero);
        auto p_plus = node.GetPartition(dafs::Node::Slot::Plus);

        auto minus_address = p_minus->GetNodeSetDetails().zero.replication;
        auto plus_address = p_plus->GetNodeSetDetails().zero.replication;

        if (minus_address.ip == address.ip &&
            minus_address.port == address.port &&
            !p_minus->IsAddressResponsive(address))
        {
            // Here we agree with the proposal that minus partition should be
            // removed.
            sender.Send(
                dafs::Message
                {
                    p_zero->GetNodeSetDetails().zero.management,
                    p_zero->GetNodeSetDetails().zero.management,
                    dafs::MessageType::_MinusExitCluster,
                    std::vector<dafs::MetaData>
                    {
                        dafs::MetaData
                        {
                            dafs::IdentityKey,
                            dafs::Serialize(p_minus->GetIdentity())
                        }
                    }
                }
            );
        }
        else if (plus_address.ip == address.ip &&
                 plus_address.port == address.port &&
                 !p_plus->IsAddressResponsive(address))
        {
            // Here we agree with the proposal that plus partition should be
            // removed.
            sender.Send(
                dafs::Message
                {
                    p_zero->GetNodeSetDetails().zero.management,
                    p_zero->GetNodeSetDetails().zero.management,
                    dafs::MessageType::_PlusExitCluster,
                    std::vector<dafs::MetaData>
                    {
                        dafs::MetaData
                        {
                            dafs::IdentityKey,
                            dafs::Serialize(p_plus->GetIdentity())
                        }
                    }
                }
            );
        }
        dafs::Message m;
        return m;
    }


    dafs::Message
    HandlePlusExitCluster(
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
            if (!p_zero->Acquire())
            {
                //
                // Did not acquire partition lock.
                //
                dafs::Message m;
                return m;
            }

            p_zero->SetPlus(
                p_plus->GetNodeSetDetails().plus.management,
                p_plus->GetNodeSetDetails().plus.replication,
                p_plus->GetNodeSetDetails().plus.identity,
                Constant::PartitionMinusName);

            //
            // Save plus partition's block info list for pruning on the other
            // server which will handle the 2nd half of the exit.
            //
            auto blockinfo_list = p_plus->GetIndex();

            sender.Send(
                dafs::Message
                {
                    p_zero->GetNodeSetDetails().zero.management,
                    p_plus->GetNodeSetDetails().plus.management,
                    dafs::MessageType::_MinusExitCluster,
                    std::vector<dafs::MetaData>
                    {
                        dafs::MetaData
                        {
                            dafs::EndpointKey,
                            dafs::Serialize(p_plus->GetNodeSetDetails().minus)
                        },
                        dafs::MetaData
                        {
                            dafs::IdentityKey,
                            dafs::Serialize(identity)
                        },
                        dafs::MetaData
                        {
                            dafs::BlockInfoListKey,
                            dafs::Serialize(blockinfo_list)
                        }
                    }
                }
            );
        }
        else
        {
            if (!p_zero->Acquire())
            {
                //
                // Did not acquire partition lock.
                //
                dafs::Message m;
                return m;
            }

            auto endpoint = metadata.GetValue<dafs::Endpoint>(dafs::EndpointKey);

            // TODO: Use blocklist to add _relavent_ files to p_plus and p_zero

            p_zero->SetPlus(
                endpoint.management,
                endpoint.replication,
                endpoint.identity,
                Constant::PartitionMinusName);

            p_plus->Release();
            p_zero->Release();
        }

        dafs::Message m;
        return m;
    }


    dafs::Message
    HandleMinusExitCluster(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        auto identity = metadata.GetValue<dafs::Identity>(dafs::IdentityKey);

        auto p_minus = node.GetPartition(dafs::Node::Slot::Minus);
        auto p_zero = node.GetPartition(dafs::Node::Slot::Zero);
        auto p_plus = node.GetPartition(dafs::Node::Slot::Plus);

        if (p_minus->GetIdentity() == identity)
        {
            if (!p_zero->Acquire())
            {
                //
                // Did not acquire partition lock.
                //
                dafs::Message m;
                return m;
            }

            p_zero->SetMinus(
                p_minus->GetNodeSetDetails().minus.management,
                p_minus->GetNodeSetDetails().minus.replication,
                p_minus->GetNodeSetDetails().minus.identity,
                Constant::PartitionPlusName);

            //
            // Save minus partition's block info list for pruning on the other
            // server which will handle the 2nd half of the exit.
            //
            auto blockinfo_list = p_minus->GetIndex();

            sender.Send(
                dafs::Message
                {
                    p_zero->GetNodeSetDetails().zero.management,
                    p_minus->GetNodeSetDetails().minus.management,
                    dafs::MessageType::_PlusExitCluster,
                    std::vector<dafs::MetaData>
                    {
                        dafs::MetaData
                        {
                            dafs::EndpointKey,
                            dafs::Serialize(p_minus->GetNodeSetDetails().plus)
                        },
                        dafs::MetaData
                        {
                            dafs::IdentityKey,
                            dafs::Serialize(identity)
                        },
                        dafs::MetaData
                        {
                            dafs::BlockInfoListKey,
                            dafs::Serialize(blockinfo_list)
                        }
                    }
                }
            );
        }
        else
        {
            if (!p_zero->Acquire())
            {
                //
                // Did not acquire partition lock.
                //
                dafs::Message m;
                return m;
            }

            auto endpoint = metadata.GetValue<dafs::Endpoint>(dafs::EndpointKey);

            // TODO: Use blocklist to add _relavent_ files to p_minus and p_zero

            p_zero->SetMinus(
                endpoint.management,
                endpoint.replication,
                endpoint.identity,
                Constant::PartitionPlusName);

            p_minus->Release();
            p_zero->Release();
        }

        dafs::Message m;
        return m;
    }
}
