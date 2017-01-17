#include "dafs/handler.hpp"


namespace dafs
{
    void
    HandleCreateFile(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        auto fileinfo = metadata.GetValue<dafs::FileInfo>(dafs::FileInfoKey);

        node.GetPartition(Node::Slot::Zero).CreateFile(fileinfo);
    }


    void
    HandleDeleteFile(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        auto fileinfo = metadata.GetValue<dafs::FileInfo>(dafs::FileInfoKey);

        node.GetPartition(Node::Slot::Zero).DeleteFile(fileinfo);
    }


    void
    HandleReadBlock(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        auto blockinfo = metadata.GetValue<dafs::BlockInfo>(dafs::BlockInfoKey);
        auto blockformat = node.GetPartition(Node::Slot::Zero).ReadBlock(blockinfo);

        // return blockformat and version
    }


    void
    HandleWriteBlock(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        auto blockinfo = metadata.GetValue<dafs::BlockInfo>(dafs::BlockInfoKey);
        auto block = metadata.GetValue<dafs::BlockFormat>(dafs::BlockFormatKey);

        node.GetPartition(Node::Slot::Zero).WriteBlock(blockinfo, block);
    }


    void HandleAllocate(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        // dafs::Allocation allocation
    }


    void HandleAllocated(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        // dafs::Allocation allocation
    }


    void HandleRequestInitiation(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        auto address = metadata.GetValue<std::string>(dafs::AddressKey);
        auto port = metadata.GetValue<short>(dafs::PortKey);

        node.GetPartition(Node::Slot::Zero).AddNode(address, port);
    }


    void HandleProcessInitation(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        // initiate a node into a cluster
    }


    void HandleConcludeInitation(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        // initiate a node into a cluster
    }
}
