#include "dafs/handler.hpp"


namespace dafs
{
    dafs::Message
    HandleAllocateBlock(
        dafs::Node& node,
        dafs::MetaDataParser metadata)
    {
        dafs::Message m;

        //
        // Allocate from the zero partition by default?
        //
        auto blockinfo = node.GetPartition(dafs::Node::Slot::Zero)->AllocateBlock();
        m.metadata.push_back(
            dafs::MetaData
            {
                dafs::BlockInfoKey,
                dafs::Serialize<dafs::BlockInfo>(blockinfo)
            }
        );
        return m;
    }


    dafs::Message
    HandleReadBlock(
        dafs::Node& node,
        dafs::MetaDataParser metadata)
    {
        auto blockinfo = metadata.GetValue<dafs::BlockInfo>(dafs::BlockInfoKey);
        auto partition = node.GetPartition(blockinfo.identity);

        dafs::Message m;
        if (partition->IsActive())
        {
            m.metadata.push_back(
                dafs::MetaData
                {
                    dafs::BlockFormatKey,
                    dafs::Serialize(partition->ReadBlock(blockinfo))
                }
            );
            m.type = dafs::MessageType::Success;
        }
        else
        {
            m.metadata = std::vector<dafs::MetaData>
            {
                dafs::MetaData
                {
                    dafs::AddressKey,
                    dafs::Serialize(
                        node.GetPartition(dafs::Node::Slot::Minus)
                            ->GetNodeSetDetails().minus.management)
                }
            };
            m.type = dafs::MessageType::Failure;
        }
        return m;
    }


    dafs::Message
    HandleWriteBlock(
        dafs::Node& node,
        dafs::MetaDataParser metadata)
    {
        auto blockinfo = metadata.GetValue<dafs::BlockInfo>(dafs::BlockInfoKey);
        auto block = metadata.GetValue<dafs::BlockFormat>(dafs::BlockFormatKey);
        auto partition = node.GetPartition(blockinfo.identity);

        dafs::Message m;
        if (partition->IsActive())
        {
            partition->WriteBlock(blockinfo, block);
            m.type = dafs::MessageType::Success;
        }
        else
        {
            m.metadata = std::vector<dafs::MetaData>
            {
                dafs::MetaData
                {
                    dafs::AddressKey,
                    dafs::Serialize(
                        node.GetPartition(dafs::Node::Slot::Minus)
                            ->GetNodeSetDetails().minus.management)
                }
            };
            m.type = dafs::MessageType::Failure;
        }
        return m;
    }


    dafs::Message
    HandleDeleteBlock(
        dafs::Node& node,
        dafs::MetaDataParser metadata)
    {
        auto blockinfo = metadata.GetValue<dafs::BlockInfo>(dafs::BlockInfoKey);
        auto partition = node.GetPartition(blockinfo.identity);

        dafs::Message m;
        if (partition->IsActive())
        {
            partition->DeleteBlock(blockinfo);
            m.type = dafs::MessageType::Success;
        }
        else
        {
            m.metadata = std::vector<dafs::MetaData>
            {
                dafs::MetaData
                {
                    dafs::AddressKey,
                    dafs::Serialize(
                        node.GetPartition(dafs::Node::Slot::Minus)
                            ->GetNodeSetDetails().minus.management)
                }
            };
            m.type = dafs::MessageType::Failure;
        }
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
                dafs::ZeroReplicatedEndpointsKey,
                dafs::Serialize<dafs::ReplicatedEndpoints>(
                    node.GetPartition(dafs::Node::Slot::Zero)->GetNodeSetDetails()
                )
            },
            dafs::MetaData
            {
                dafs::PlusReplicatedEndpointsKey,
                dafs::Serialize<dafs::ReplicatedEndpoints>(
                    node.GetPartition(dafs::Node::Slot::Plus)->GetNodeSetDetails()
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

        // TODO: Add check that block list on node is empty in order to ensure
        //       that add server stays O(1) complexity. Then refuse to join
        //       cluster if list is non-empty.

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
                //
                // Distributed locks here are used to temporarily prevent
                // decrees from being proposed by other nodes during add node.
                // The issue is that the current replication library
                // implementation requires add replica to be an undisrupted
                // atomic operation.  If for example we have replica set
                // {a,b,c} and add replica {d}, then during the bootstrap while
                // files are being copied to {d}, no further decrees can edit
                // the distributed files else {d} may end up in corrupted
                // split-brain state.
                //
                // FIXME: This also means that other replicated write
                //        operations, like write block, must respect this lock.
                //
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

        for (auto info : SplitUpperIndex(p_minus->GetIndex(),
                                         p_minus->GetIdentity(),
                                         p_zero->GetIdentity(),
                                         p_plus->GetIdentity()).items)
        {
            auto format = p_minus->ReadBlock(info);
            p_zero->WriteBlock(info, format);
            p_minus->DeleteBlock(info);
        }

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

            //
            // Update zero partition to contain blocks that used to be owned by
            // partition plus. And remove blocks no longer owned by plus
            // partition.
            //
            for (const auto& info : p_plus->GetIndex().items)
            {
                auto format = p_plus->ReadBlock(info);
                p_zero->WriteBlock(info, format);
                p_plus->DeleteBlock(info);
            }

            p_zero->SetPlus(
                p_plus->GetNodeSetDetails().plus.management,
                p_plus->GetNodeSetDetails().plus.replication,
                p_plus->GetNodeSetDetails().plus.identity,
                Constant::PartitionMinusName);

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
                        }
                    }
                }
            );
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

        if (p_minus->GetIdentity() != identity)
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
