#include "storage.hpp"


namespace dafs
{
    Storage::Storage(std::shared_ptr<Persistence> persister_)
        : persister(persister_)
    {
    }


    void
    Storage::CreateFile(FileInfo info)
    {
        persister->Insert(SuperBlock, info);
    }


    void
    Storage::OpenFile(FileInfo info)
    {
    }


    void
    Storage::DeleteFile(FileInfo info)
    {
    }


    void
    Storage::ReadFile(FileInfo info)
    {
    }


    void
    Storage::WriteFile(FileInfo info, Bytes data)
    {
    }


    void
    Storage::CreateBlock(BlockInfo info)
    {
    }


    void
    Storage::DeleteBlock(BlockInfo info)
    {
    }


    void
    Storage::ReadBlock(BlockInfo info)
    {
        BlockFormat block = persister->Get(info);
    }


    void
    Storage::WriteBlock(BlockInfo info, Bytes data)
    {
        // TODO: Handle overflows to another block.
        persister->Write(info, data);
    }
}
