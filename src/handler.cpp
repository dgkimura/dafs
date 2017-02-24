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
        dafs::NetworkSender searcher(
            metadata.GetValue<dafs::Address>(dafs::AddressKey));

        auto p_minus = node.GetPartition(dafs::Node::Slot::Minus);
        auto p_zero = node.GetPartition(dafs::Node::Slot::Zero);
        auto p_plus = node.GetPartition(dafs::Node::Slot::Plus);

        searcher.Send(
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
        auto identity = dafs::Identity(metadata.GetValue<std::string>(
            dafs::IdentityKey));
        auto details = metadata.GetValue<dafs::NodeDetails>(dafs::NodeDetailsKey);


        auto p_minus = node.GetPartition(dafs::Node::Slot::Minus);
        auto p_zero = node.GetPartition(dafs::Node::Slot::Zero);

        dafs::NetworkSender sender(
            metadata.GetValue<dafs::Address>(dafs::AddressKey));

        if (identity < p_minus->GetDetails().identity ||
            identity > p_zero->GetDetails().identity)
        {
            //
            // Reject initiation request by telling node who to ask next.
            //
            sender.Send(
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
            //
            // Accept initiation request and update half of topology.
            //

            // Push replication onto initiated node.
            p_minus->AddNode(details.minus_details.interface);

            // Delete extra replications.
            p_minus->RemoveNode(p_minus->GetDetails().interface);

            // TODO: Remove artifacts in p_minus partition.

            sender.Send(
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


        auto p_plus = node.GetPartition(dafs::Node::Slot::Plus);
        auto p_zero = node.GetPartition(dafs::Node::Slot::Zero);

        dafs::NetworkSender sender(
            metadata.GetValue<dafs::Address>(dafs::AddressKey));

        if (identity > p_plus->GetDetails().identity ||
            identity < p_zero->GetDetails().identity)
        {
            //
            // Foobar'd topology.
            //
        }
        else
        {
            //
            // Update second half of topology
            //

            // Push replication onto initiated node.
            p_plus->AddNode(details.plus_details.interface);

            // Delete extra replications.
            p_plus->RemoveNode(p_plus->GetDetails().interface);

            // TODO: Remove artifacts in p_plus partition.

            // Send accepted messge to ndoe.
            sender.Send(
                dafs::Message
                {
                    p_zero->GetDetails().author,
                    address,
                    dafs::MessageType::_AcceptInitation,
                    std::vector<dafs::MetaData>
                    {
                        dafs::MetaData
                        {
                        }
                    }
                }
            );
        }

        dafs::Message m;
        return m;
    }


    dafs::Message
    HandleAcceptInitiation(
        dafs::Node& node,
        dafs::MetaDataParser metadata)
    {
        // initiate a node into a cluster
        dafs::Message m;
        return m;
    }


    dafs::Message
    HandleConcludeInitation(
        dafs::Node& node,
        dafs::MetaDataParser metadata)
    {
        // initiate a node into a cluster
        dafs::Message m;
        return m;
    }
}
