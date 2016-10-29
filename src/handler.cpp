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
    HandleOpenFile(
        dafs::Storage& store,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        auto fileinfo = metadata.GetValue<dafs::FileInfo>();
        store.OpenFile(fileinfo);
    }


    void
    HandleReadFile(
        dafs::Storage& store,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        auto fileinfo = metadata.GetValue<dafs::FileInfo>();
        store.ReadFile(fileinfo);
    }


    void
    HandleWriteFile(
        dafs::Storage& store,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        auto fileinfo = metadata.GetValue<dafs::FileInfo>();
        auto bytes = metadata.GetValue<dafs::Bytes>();
        store.WriteFile(fileinfo, bytes);
    }


    void
    HandleReadBlock(
        dafs::Storage& store,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        auto blockinfo = metadata.GetValue<dafs::BlockInfo>();
        store.ReadBlock(blockinfo);
    }


    void
    HandleWriteBlock(
        dafs::Storage& store,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        auto blockinfo = metadata.GetValue<dafs::BlockInfo>();
        auto bytes = metadata.GetValue<dafs::Bytes>();
        store.WriteBlock(blockinfo, bytes);
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
