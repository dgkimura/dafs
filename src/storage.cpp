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
    Persister::Put(BlockInfo info, Delta delta)
    {
        std::string proposal = Serialize<dafs::Delta>(delta);
        parliament.CreateProposal(proposal);
    }


    Storage::Storage(Persister persister_)
        : persister(persister_)
    {
    }


    BlockFormat
    Storage::ReadBlock(BlockInfo info)
    {
        return persister.Get(info);
    }


    void
    Storage::Write(BlockInfo info, Bytes data)
    {
        BlockFormat was = persister.Get(info);
        BlockFormat is(was);

        std::memmove(is.contents + info.offset, data.content, data.size);

        Delta delta = CreateDelta(info.filename, was.contents, is.contents);
        persister.Put(info, delta);
    }
}
