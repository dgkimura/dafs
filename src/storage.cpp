#include "diflib.h"

#include "delta.hpp"
#include "messages.hpp"
#include "serialization.hpp"
#include "storage.hpp"


namespace dafs
{
    std::string Diff(std::string a, std::string b)
    {
        return "";
    }


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
    Persister::Update(BlockInfo info, BlockFormat was, BlockFormat is)
    {
        dafs::Delta delta
        {
            info.filename,
            Diff(is.contents, was.contents)
        };
        std::string proposal = Serialize<dafs::Delta>(delta);
        parliament.CreateProposal(proposal);
    }


    Storage::Storage(Loader loader_, Persister persister_)
        : loader(loader_), persister(persister_)
    {
    }


    void
    Storage::Save(BlockInfo info, BlockFormat block)
    {
        BlockFormat was = loader.Fetch(info);
        persister.Update(info, was, block);
    }


    BlockFormat
    Storage::Fetch(BlockInfo info)
    {
        return loader.Fetch(info);
    }
}
