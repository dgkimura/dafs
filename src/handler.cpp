#include "dafs/handler.hpp"


namespace dafs
{
    dafs::Message
    HandleReadBlock(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
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
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        auto blockinfo = metadata.GetValue<dafs::BlockInfo>(dafs::BlockInfoKey);
        auto block = metadata.GetValue<dafs::BlockFormat>(dafs::BlockFormatKey);

        node.GetPartition(blockinfo.identity)->WriteBlock(blockinfo, block);
        dafs::Message m;
        return m;
    }


    dafs::Message
    HandleGetNodeDetails(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
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
    HandleRequestInitiation(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        auto ip = metadata.GetValue<std::string>(dafs::AddressKey);
        auto port = metadata.GetValue<short>(dafs::PortKey);
        auto identity = dafs::Identity(metadata.GetValue<std::string>(
            dafs::IdentityKey));

        node.GetPartition(Node::Slot::Zero)->AddNode(dafs::Address(ip, port));
        dafs::Message m;
        return m;
    }


    dafs::Message
    HandleProcessInitation(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        // initiate a node into a cluster
        dafs::Message m;
        return m;
    }


    dafs::Message
    HandleConcludeInitation(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        // initiate a node into a cluster
        dafs::Message m;
        return m;
    }
}
