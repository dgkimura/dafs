#include <boost/filesystem.hpp>

#include "messages.hpp"
#include "serialization.hpp"
#include "storage.hpp"


namespace dafs
{
    Durable::Durable(Parliament parliament_, std::string dirname_)
        : parliament(parliament_),
          dirname(dirname_)
    {
    }


    BlockFormat
    Durable::Get(BlockInfo info)
    {
        std::fstream f((fs::path(dirname) / fs::path(info.filename)).string(),
                        std::ios::out | std::ios::in | std::ios::binary);
        BlockFormat b;
        f.read(b.contents, BLOCK_SIZE_IN_BYTES);
        return b;
    }


    void
    Durable::Put(BlockInfo info, Bytes data)
    {
        BlockFormat was = Get(info);
        BlockFormat is(was);

        std::memmove(is.contents + info.offset, data.contents,
                     BLOCK_SIZE_IN_BYTES - info.offset);

        Delta delta = CreateDelta(info.filename, was.contents, is.contents);

        parliament.CreateProposal(Serialize<dafs::Delta>(delta));
    }


    Storage::Storage(std::shared_ptr<Persistence> persister_)
        : persister(persister_)
    {
    }


    void
    Storage::CreateFile(FileInfo info)
    {
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
        persister->Put(info, data);
    }
}
