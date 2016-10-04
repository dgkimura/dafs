#include "messages.hpp"
#include "serialization.hpp"
#include "storage.hpp"


namespace dafs
{
    BlockFormat
    Loader::Fetch(BlockInfo info)
    {
        std::ifstream f(info.filename);
        BlockFormat b;
        f.read(b.contents, BLOCK_SIZE_IN_BYTES);
        return b;
    }


    Persister::Persister(Parliament parliament_)
        : parliament(parliament_)
    {
    }


    void
    Persister::Update(BlockInfo info, Delta delta)
    {
        std::string proposal = Serialize<dafs::Delta>(delta);
        parliament.CreateProposal(proposal);
    }


    Storage::Storage(Loader loader_, Persister persister_)
        : loader(loader_), persister(persister_)
    {
    }


    BlockFormat
    Storage::Load(BlockInfo info)
    {
        return loader.Fetch(info);
    }


    void
    Storage::Save(BlockInfo info, BlockFormat is)
    {
        BlockFormat was = loader.Fetch(info);
        Delta delta = CreateDelta(info.filename, was.contents, is.contents);
        persister.Update(info, delta);
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
