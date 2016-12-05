#include "handler.hpp"


namespace dafs
{
    void
    HandleCreateFile(
        dafs::Storage& store,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        auto fileinfo = metadata.GetValue<dafs::FileInfo>();
        store.CreateFile(fileinfo);
    }


    void
    HandleDeleteFile(
        dafs::Storage& store,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        auto fileinfo = metadata.GetValue<dafs::FileInfo>();
        store.DeleteFile(fileinfo);
    }


    void
    HandleReadBlock(
        dafs::Storage& store,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        auto blockinfo = metadata.GetValue<dafs::BlockInfo>();
        auto blockformat = store.ReadBlock(blockinfo);

        // return blockformat and version
    }


    void
    HandleWriteBlock(
        dafs::Storage& store,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        auto blockinfo = metadata.GetValue<dafs::BlockInfo>();
        auto block = metadata.GetValue<dafs::BlockFormat>();
        store.WriteBlock(blockinfo, block);
    }


    void HandleAllocate(
        dafs::Storage& store,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        // dafs::Allocation allocation
    }


    void HandleAllocated(
        dafs::Storage& store,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        // dafs::Allocation allocation
    }
}
