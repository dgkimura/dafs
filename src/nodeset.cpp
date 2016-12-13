#include <cstring>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

#include "commit.hpp"
#include "customhash.hpp"
#include "nodeset.hpp"
#include "storage.hpp"


namespace dafs
{
    ReplicatedNodeSet::ReplicatedNodeSet(
        Parliament& parliament,
        dafs::BlockInfo nodeset_info,
        dafs::Signal& in_progress)
    : parliament(parliament),
      nodeset_info(nodeset_info),
      in_progress(in_progress)
    {
    }


    ReplicatedNodeSet::ReplicatedNodeSet(
        const ReplicatedNodeSet& other)
    : parliament(other.parliament),
      nodeset_info(other.nodeset_info),
      in_progress(other.in_progress)
    {
    }


    void
    ReplicatedNodeSet::AddNode(std::string address, short port)
    {
        std::string item(address + ":" + std::to_string(port));

        propose_update(
            nodeset_info,
            item,
            dafs::ProposalType::AddNode);
    }


    void
    ReplicatedNodeSet::RemoveNode(std::string address, short port)
    {
        std::string item(address + ":" + std::to_string(port));

        propose_update(
            nodeset_info,
            item,
            dafs::ProposalType::RemoveNode);
    }


    void
    ReplicatedNodeSet::propose_update(
        BlockInfo info,
        std::string data,
        dafs::ProposalType proposal_type)
    {
        info.path = fs::path(info.path).string();

        parliament.SendProposal
        (
            dafs::Serialize<dafs::Proposal>
            (
                CreateProposal
                (
                    proposal_type,
                    data,
                    info,
                    info.revision
                )
            )
        );
        in_progress.Wait();
    }
}
