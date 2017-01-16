#include "dafs/handler.hpp"


namespace dafs
{
    void
    HandleCreateFile(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        auto fileinfo = metadata.GetValue<dafs::FileInfo>();
        node.GetPartition(Node::Slot::Zero).CreateFile(fileinfo);
    }


    void
    HandleDeleteFile(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        auto fileinfo = metadata.GetValue<dafs::FileInfo>();
        node.GetPartition(Node::Slot::Zero).DeleteFile(fileinfo);
    }


    void
    HandleReadBlock(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        auto blockinfo = metadata.GetValue<dafs::BlockInfo>();
        auto blockformat = node.GetPartition(Node::Slot::Zero).ReadBlock(blockinfo);

        // return blockformat and version
    }


    void
    HandleWriteBlock(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        auto blockinfo = metadata.GetValue<dafs::BlockInfo>();
        auto block = metadata.GetValue<dafs::BlockFormat>();
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
        // initiate a node into a cluster
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
