#include <cstring>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

#include "dafs/commit.hpp"
#include "dafs/customhash.hpp"
#include "dafs/disk.hpp"
#include "dafs/replicated.hpp"
#include "dafs/serialization.hpp"
#include "dafs/signal.hpp"


namespace dafs
{
    ReplicatedStorage::ReplicatedStorage(
        Parliament& parliament,
        dafs::Signal& in_progress)
    : parliament(parliament),
      in_progress(in_progress)
    {
    }


    ReplicatedStorage::ReplicatedStorage(
        const ReplicatedStorage& other)
    : parliament(other.parliament),
      in_progress(other.in_progress)
    {
    }


    BlockFormat
    ReplicatedStorage::ReadBlock(BlockInfo info)
    {
        return dafs::ReadBlock(info);
    }


    void
    ReplicatedStorage::DeleteBlock(BlockInfo info)
    {
        parliament.SendProposal
        (
            dafs::Serialize<dafs::Proposal>
            (
                CreateProposal
                (
                    dafs::ProposalType::DeleteBlock,
                    "",
                    info,
                    info.revision
                )
            )
        );
        in_progress.Wait();
    }


    void
    ReplicatedStorage::WriteBlock(BlockInfo info, BlockFormat data)
    {
        BlockFormat was = dafs::ReadBlock(info);
        Delta delta = CreateDelta(info.path, was.contents, data.contents);

        do_write(info, dafs::Serialize(delta));
    }


    void
    ReplicatedStorage::Write(BlockInfo info, Delta delta)
    {
        do_write(info, dafs::Serialize(delta));
    }


    void
    ReplicatedStorage::do_write(
        BlockInfo info,
        std::string data)
    {
        parliament.SendProposal
        (
            dafs::Serialize<dafs::Proposal>
            (
                CreateProposal
                (
                    dafs::ProposalType::WriteBlock,
                    data,
                    info,
                    info.revision
                )
            )
        );
        in_progress.Wait();
    }


    ReplicatedNodeSet::ReplicatedNodeSet(
        Parliament& parliament,
        dafs::Signal& in_progress)
    : parliament(parliament),
      in_progress(in_progress)
    {
    }


    ReplicatedNodeSet::ReplicatedNodeSet(
        const ReplicatedNodeSet& other)
    : parliament(other.parliament),
      in_progress(other.in_progress)
    {
    }


    void
    ReplicatedNodeSet::AddNode(dafs::Address address)
    {
        parliament.AddLegislator(address.ip, address.port);
        in_progress.Wait();
    }


    void
    ReplicatedNodeSet::RemoveNode(dafs::Address address)
    {
        parliament.RemoveLegislator(address.ip, address.port);
        in_progress.Wait();
    }
}
