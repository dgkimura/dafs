#include "handler.hpp"


namespace dafs
{
    void
    HandleCreateFile(
        dafs::Storage store,
        std::vector<dafs::MetaData> metadata)
    {
        // dafs::FileInfo info
    }


    void
    HandleDeleteFile(
        dafs::Storage store,
        std::vector<dafs::MetaData> metadata)
    {
        // dafs::FileInfo info
    }


    void
    HandleOpenFile(
        dafs::Storage store,
        std::vector<dafs::MetaData> metadata)
    {
        // dafs::FileInfo info
    }


    void
    HandleReadFile(
        dafs::Storage store,
        std::vector<dafs::MetaData> metadata)
    {
        // dafs::FileInfo info
    }


    void
    HandleWriteFile(
        dafs::Storage store,
        std::vector<dafs::MetaData> metadata)
    {
        // dafs::Bytes bytes
        // dafs::FileInfo info
    }


    void
    HandleReadBlock(
        dafs::Storage store,
        std::vector<dafs::MetaData> metadata)
    {
        // dafs::BlockInfo info
    }


    void
    HandleWriteBlock(
        dafs::Storage store,
        std::vector<dafs::MetaData> metadata)
    {
        // dafs::Bytes bytes
        // dafs::BlockInfo info
    }


    void HandleAllocate(
        dafs::Storage store,
        std::vector<dafs::MetaData> metadata)
    {
        // dafs::Allocation allocation
    }


    void HandleAllocated(
        dafs::Storage store,
        std::vector<dafs::MetaData> metadata)
    {
        // dafs::Allocation allocation
    }
}
