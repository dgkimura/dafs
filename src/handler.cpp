#include "handler.hpp"


namespace dafs
{
    void
    HandleCreateFile(
        dafs::Storage store,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        auto fileinfo = metadata.GetValue<dafs::FileInfo>(dafs::FileInfoKey);
        store.CreateFile(*fileinfo);
    }


    void
    HandleDeleteFile(
        dafs::Storage store,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        auto fileinfo = metadata.GetValue<dafs::FileInfo>(dafs::FileInfoKey);
        store.DeleteFile(*fileinfo);
    }


    void
    HandleOpenFile(
        dafs::Storage store,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        auto fileinfo = metadata.GetValue<dafs::FileInfo>(dafs::FileInfoKey);
        store.OpenFile(*fileinfo);
    }


    void
    HandleReadFile(
        dafs::Storage store,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        auto fileinfo = metadata.GetValue<dafs::FileInfo>(dafs::FileInfoKey);
        store.ReadFile(*fileinfo);
    }


    void
    HandleWriteFile(
        dafs::Storage store,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        auto fileinfo = metadata.GetValue<dafs::FileInfo>(dafs::FileInfoKey);
        auto bytes = metadata.GetValue<dafs::Bytes>(dafs::BytesKey);
        store.WriteFile(*fileinfo, *bytes);
    }


    void
    HandleReadBlock(
        dafs::Storage store,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        auto blockinfo = metadata.GetValue<dafs::BlockInfo>(dafs::BlockInfoKey);
        store.ReadBlock(*blockinfo);
    }


    void
    HandleWriteBlock(
        dafs::Storage store,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        auto blockinfo = metadata.GetValue<dafs::BlockInfo>(dafs::BlockInfoKey);
        auto bytes = metadata.GetValue<dafs::Bytes>(dafs::BytesKey);
        store.WriteBlock(*blockinfo, *bytes);
    }


    void HandleAllocate(
        dafs::Storage store,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        // dafs::Allocation allocation
    }


    void HandleAllocated(
        dafs::Storage store,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        // dafs::Allocation allocation
    }
}
