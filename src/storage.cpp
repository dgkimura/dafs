#include "storage.hpp"


namespace dafs
{
    std::string Diff(std::string a, std::string b)
    {
        return "";
    }


    void
    Loader::Fetch(Block& block)
    {
    }


    void
    ReplicatedStorage::Update(Block was, Block is)
    {
        std::string proposal = Diff(is.contents, was.contents);
        parliament.CreateProposal(proposal);
    }


    void
    ReplicatedStorage::Get(Block block)
    {
        loader.Fetch(block);
    }
}
