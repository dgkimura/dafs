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


    Block
    Loader::Fetch(BlockInfo info)
    {
        std::ifstream f(info.filename);
        std::stringstream buffer;
        buffer << f.rdbuf();
        Block b(buffer.str());
        return b;
    }


    Persister::Persister(Parliament parliament_)
        : parliament(parliament_)
    {
    }


    void
    Persister::Update(BlockInfo info, Block was, Block is)
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
    Storage::Save(BlockInfo info, Block block)
    {
        Block was = loader.Fetch(info);
        persister.Update(info, was, block);
    }


    Block
    Storage::Fetch(BlockInfo info)
    {
        return loader.Fetch(info);
    }
}
