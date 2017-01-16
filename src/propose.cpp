#include "dafs/customhash.hpp"
#include "dafs/propose.hpp"
#include "dafs/serialization.hpp"


namespace dafs
{
    Proposal
    CreateProposal(
        ProposalType type,
        std::string item,
        BlockInfo info,
        int revision)
    {
        ProposalContent edit;
        edit.info = info;
        edit.change = item;
        edit.hash = std::hash<dafs::BlockInfo>{}(info);

        Proposal p;
        p.type = type;
        p.content = dafs::Serialize(edit);
        return p;
    }
}
