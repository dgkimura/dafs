#include "messages.hpp"
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
    Storage::Save(BlockInfo info, Block block)
    {
        Block was = loader.Fetch(info);
        persister.Update(was, block);
    }


    Block
    Storage::Fetch(BlockInfo info)
    {
        return loader.Fetch(info);
    }
}
