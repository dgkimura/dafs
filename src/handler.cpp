#include "dafs/handler.hpp"


namespace dafs
{
    void
    HandleCreateFile(
        dafs::Partition& partition,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        auto fileinfo = metadata.GetValue<dafs::FileInfo>();
        partition.CreateFile(fileinfo);
    }


    void
    HandleDeleteFile(
        dafs::Partition& partition,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        auto fileinfo = metadata.GetValue<dafs::FileInfo>();
        partition.DeleteFile(fileinfo);
    }


    void
    HandleReadBlock(
        dafs::Partition& partition,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        auto blockinfo = metadata.GetValue<dafs::BlockInfo>();
        auto blockformat = partition.ReadBlock(blockinfo);

        // return blockformat and version
    }


    void
    HandleWriteBlock(
        dafs::Partition& partition,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        auto blockinfo = metadata.GetValue<dafs::BlockInfo>();
        auto block = metadata.GetValue<dafs::BlockFormat>();
        partition.WriteBlock(blockinfo, block);
    }


    void HandleAllocate(
        dafs::Partition& partition,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        // dafs::Allocation allocation
    }


    void HandleAllocated(
        dafs::Partition& partition,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        // dafs::Allocation allocation
    }


    void HandleInitiateCluster(
        dafs::Partition& partition,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        // initiate a node into a cluster
    }
}
