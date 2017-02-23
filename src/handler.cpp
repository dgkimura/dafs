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
        dafs::Message search_message
        {
            node.GetPartition(dafs::Node::Slot::Zero)->GetDetails().author,
            metadata.GetValue<dafs::Address>(dafs::AddressKey),
            dafs::MessageType::GetNodeDetails
        };

        auto details = node.GetPartition(dafs::Node::Slot::Zero)->GetDetails();

        for (;;)
        {
            dafs::NetworkSender searcher(search_message.to);
            searcher.Send(search_message);
            auto response_message = searcher.Receive();
            dafs::MetaDataParser parser(response_message.metadata);
            auto d = parser.GetValue<dafs::NodeDetails>(dafs::NodeDetailsKey);
            if (d.minus_details.identity < details.identity &&
                details.identity < d.plus_details.identity)
            {
                break;
            }
            search_message.to = node.GetPartition(dafs::Node::Slot::Plus)
                                     ->GetDetails().author;
        }
        dafs::NetworkSender joiner(search_message.to);
        joiner.Send(
            dafs::Message
            {
                details.author,
                search_message.to,
                dafs::MessageType::_RequestInitiation,
                std::vector<dafs::MetaData>
                {
                    dafs::MetaData
                    {
                        dafs::IdentityKey,
                        dafs::Serialize(details.identity)
                    }
                }
            }
        );

        dafs::Message m;
        return m;
    }


    dafs::Message
    HandleRequestInitiation(
        dafs::Node& node,
        dafs::MetaDataParser metadata)
    {
        auto address = metadata.GetValue<dafs::Address>(dafs::AddressKey);
        auto identity = dafs::Identity(metadata.GetValue<std::string>(
            dafs::IdentityKey));

        node.GetPartition(Node::Slot::Zero)->AddNode(address);
        dafs::Message m;
        return m;
    }


    dafs::Message
    HandleProcessInitation(
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
