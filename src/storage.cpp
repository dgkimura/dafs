#include "messages.hpp"
#include "serialization.hpp"
#include "storage.hpp"


namespace dafs
{
    Durable::Durable(Parliament parliament_)
        : parliament(parliament_)
    {
    }


    BlockFormat
    Durable::Get(BlockInfo info)
    {
        std::ifstream f(info.filename);
        BlockFormat b;
        f.read(b.contents, BLOCK_SIZE_IN_BYTES);
        return b;
    }


    void
    Durable::Put(BlockInfo info, Delta delta)
    {
        std::string proposal = Serialize<dafs::Delta>(delta);
        parliament.CreateProposal(proposal);
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
        BlockFormat was = persister->Get(info);
        BlockFormat is(was);

        std::memmove(is.contents + info.offset, data.contents, BLOCK_SIZE_IN_BYTES);

        Delta delta = CreateDelta(info.filename, was.contents, is.contents);
        persister->Put(info, delta);
    }
}
