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
        dafs::MetaDataParser metadata)
    {
        dafs::NetworkSender reply(
            metadata.GetValue<dafs::Address>(dafs::AddressKey));

        auto p_minus = node.GetPartition(dafs::Node::Slot::Minus);
        auto p_zero = node.GetPartition(dafs::Node::Slot::Zero);
        auto p_plus = node.GetPartition(dafs::Node::Slot::Plus);

        reply.Send(
            dafs::Message
            {
                node.GetPartition(dafs::Node::Slot::Zero)->GetDetails().author,
                metadata.GetValue<dafs::Address>(dafs::AddressKey),
                dafs::MessageType::_RequestMinusInitiation,
                std::vector<dafs::MetaData>
                {
                    dafs::MetaData
                    {
                        dafs::NodeDetailsKey,
                        dafs::Serialize(
                            dafs::NodeDetails
                            {
                                p_minus->GetDetails(),
                                p_zero->GetDetails(),
                                p_plus->GetDetails()
                            }
                        )
                    },
                    dafs::MetaData
                    {
                        dafs::AddressKey,
                        dafs::Serialize(p_zero->GetDetails().author)
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
        dafs::MetaDataParser metadata)
    {
        auto address = metadata.GetValue<dafs::Address>(dafs::AddressKey);
        auto details = metadata.GetValue<dafs::NodeDetails>(dafs::NodeDetailsKey);
        auto identity = details.zero_details.identity;


        auto p_minus = node.GetPartition(dafs::Node::Slot::Minus);
        auto p_zero = node.GetPartition(dafs::Node::Slot::Zero);
        auto p_plus = node.GetPartition(dafs::Node::Slot::Plus);

        dafs::NetworkSender reply(
            metadata.GetValue<dafs::Address>(dafs::AddressKey));

        if (p_minus->IsActive() &&
            (!IsLogicallyOrdered(p_minus->GetDetails().identity,
                                 identity,
                                 p_zero->GetDetails().identity)))
        {
            //
            // Reject initiation request by telling node who to ask next.
            //
            reply.Send(
                dafs::Message
                {
                    p_zero->GetDetails().author,
                    address,
                    dafs::MessageType::_JoinCluster,
                    std::vector<dafs::MetaData>
                    {
                        dafs::MetaData
                        {
                            dafs::Serialize(address),
                            dafs::Serialize(p_minus->GetDetails().author)
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
                p_minus->AddNode(
                    details.plus_details.interface,
                    Constant::PartitionPlusName);

                // Delete extra replications.
                p_minus->RemoveNode(p_minus->GetDetails().interface);

                // Clear previous artifacts from partition.
                p_minus->Clear();

                dafs::NetworkSender forwarder(p_minus->GetDetails().author);
                forwarder.Send(
                    dafs::Message
                    {
                        p_zero->GetDetails().author,
                        p_minus->GetDetails().author,
                        dafs::MessageType::_RequestPlusInitiation,
                        std::vector<dafs::MetaData>
                        {
                            dafs::MetaData
                            {
                                dafs::NodeDetailsKey,
                                dafs::Serialize(details)
                            }
                        }
                    }
                );
            }
            else
            {
                //
                // Case of standalone node - accept initiation request and
                // update both halves of topology.
                //
                p_zero->AddNode(
                    details.plus_details.interface,
                    Constant::PartitionMinusName);
                dafs::NetworkSender prereply(
                    metadata.GetValue<dafs::Address>(dafs::AddressKey));
                prereply.Send(
                    dafs::Message
                    {
                        p_zero->GetDetails().author,
                        address,
                        dafs::MessageType::_AcceptPlusInitiation,
                        std::vector<dafs::MetaData>
                        {
                            dafs::MetaData
                            {
                                dafs::AddressKey,
                                dafs::Serialize(
                                    p_plus->GetDetails().interface
                                )
                            }
                        }
                    }
                );
            }

            p_zero->AddNode(
                details.minus_details.interface,
                Constant::PartitionPlusName);
            reply.Send(
                dafs::Message
                {
                    p_zero->GetDetails().author,
                    address,
                    dafs::MessageType::_AcceptMinusInitiation,
                    std::vector<dafs::MetaData>
                    {
                        dafs::MetaData
                        {
                            dafs::AddressKey,
                            dafs::Serialize(
                                p_minus->GetDetails().interface
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
    HandleRequestPlusInitiation(
        dafs::Node& node,
        dafs::MetaDataParser metadata)
    {
        auto address = metadata.GetValue<dafs::Address>(dafs::AddressKey);
        auto identity = dafs::Identity(metadata.GetValue<std::string>(
            dafs::IdentityKey));
        auto details = metadata.GetValue<dafs::NodeDetails>(dafs::NodeDetailsKey);

        auto p_zero = node.GetPartition(dafs::Node::Slot::Zero);
        auto p_plus = node.GetPartition(dafs::Node::Slot::Plus);

        dafs::NetworkSender reply(
            metadata.GetValue<dafs::Address>(dafs::AddressKey));

        if (!IsLogicallyOrdered(p_zero->GetDetails().identity,
                                identity,
                                p_plus->GetDetails().identity))
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
            p_plus->AddNode(
                details.minus_details.interface,
                Constant::PartitionMinusName);

            // Delete extra replications.
            p_plus->RemoveNode(p_plus->GetDetails().interface);

            // Clear previous artifacts from partition.
            p_plus->Clear();

            // Send accepted messge to ndoe.
            reply.Send(
                dafs::Message
                {
                    p_zero->GetDetails().author,
                    address,
                    dafs::MessageType::_AcceptPlusInitiation,
                    std::vector<dafs::MetaData>
                    {
                        dafs::MetaData
                        {
                            dafs::AddressKey,
                            dafs::Serialize(p_plus->GetDetails().interface)
                        }
                    }
                }
            );
        }

        dafs::Message m;
        return m;
    }


    dafs::Message
    HandleAcceptMinusInitiation(
        dafs::Node& node,
        dafs::MetaDataParser metadata)
    {
        auto address = metadata.GetValue<dafs::Address>(dafs::AddressKey);
        auto p_zero = node.GetPartition(dafs::Node::Slot::Zero);

        p_zero->AddNode(address, Constant::PartitionPlusName);

        dafs::Message m;
        return m;
    }


    dafs::Message
    HandleAcceptPlusInitiation(
        dafs::Node& node,
        dafs::MetaDataParser metadata)
    {
        auto address = metadata.GetValue<dafs::Address>(dafs::AddressKey);
        auto p_zero = node.GetPartition(dafs::Node::Slot::Zero);

        p_zero->AddNode(address, Constant::PartitionMinusName);

        dafs::Message m;
        return m;
    }
}
