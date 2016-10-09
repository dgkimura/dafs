#include "messages.hpp"
#include "serialization.hpp"
#include "storage.hpp"


namespace dafs
{
    Persister::Persister(Parliament parliament_)
        : parliament(parliament_)
    {
    }


    BlockFormat
    Persister::Get(BlockInfo info)
    {
        std::ifstream f(info.filename);
        BlockFormat b;
        f.read(b.contents, BLOCK_SIZE_IN_BYTES);
        return b;
    }


    void
    Persister::Set(BlockInfo info, Delta delta)
    {
        std::string proposal = Serialize<dafs::Delta>(delta);
        parliament.CreateProposal(proposal);
    }


    Storage::Storage(Persister persister_)
        : persister(persister_)
    {
    }


    BlockFormat
    Storage::Load(BlockInfo info)
    {
        return persister.Get(info);
    }


    void
    Storage::Save(BlockInfo info, BlockFormat is)
    {
        BlockFormat was = persister.Get(info);
        Delta delta = CreateDelta(info.filename, was.contents, is.contents);
        persister.Set(info, delta);
    }


    void
    Storage::Write(BlockInfo info, int offset, std::string data)
    {
        // TODO: write to block
    }


    std::string
    Storage::Read(BlockInfo info, int offset, int bytes)
    {
        // TODO: read to block
        std::string content;
        return content;
    }
}
