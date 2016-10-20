#include "handler.hpp"


namespace dafs
{
    void
    HandleCreateFile(
        dafs::Storage store,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        // dafs::FileInfo info
    }


    void
    HandleDeleteFile(
        dafs::Storage store,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        // dafs::FileInfo info
    }


    void
    HandleOpenFile(
        dafs::Storage store,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        // dafs::FileInfo info
    }


    void
    HandleReadFile(
        dafs::Storage store,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        // dafs::FileInfo info
    }


    void
    HandleWriteFile(
        dafs::Storage store,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        // dafs::Bytes bytes
        // dafs::FileInfo info
    }


    void
    HandleReadBlock(
        dafs::Storage store,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        // dafs::BlockInfo info
    }


    void
    HandleWriteBlock(
        dafs::Storage store,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender)
    {
        // dafs::Bytes bytes
        // dafs::BlockInfo info
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
