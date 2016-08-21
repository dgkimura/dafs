#include "storage.hpp"


namespace dafs
{
    std::string Diff(std::string a, std::string b)
    {
        return "";
    }


    void
    ReplicatedStorage::Update(Block was, Block is)
    {
        std::string proposal = Diff(is.contents.contents, was.contents.contents);
        parliament.CreateProposal(proposal);
    }
}
