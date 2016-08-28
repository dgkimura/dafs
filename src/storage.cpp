#include "storage.hpp"


namespace dafs
{
    std::string Diff(std::string a, std::string b)
    {
        return "";
    }


    Block
    Loader::Fetch(Block block)
    {
        Block b(block.id, block.revision, block.owner, block.contents);
        return b;
    }


    Persister::Persister(Parliament parliament_)
        : parliament(parliament_)
    {
    }


    void
    Persister::Update(Block was, Block is)
    {
        std::string proposal = Diff(is.contents, was.contents);
        parliament.CreateProposal(proposal);
    }


    Storage::Storage(Loader loader_, Persister persister_)
        : loader(loader_), persister(persister_)
    {
    }


    void
    Storage::Save(Block block)
    {
        Block was = loader.Fetch(block);
        persister.Update(was, block);
    }


    Block
    Storage::Fetch(Block block)
    {
        return loader.Fetch(block);
    }
}
