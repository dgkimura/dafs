#include <cstring>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

#include "commit.hpp"
#include "customhash.hpp"
#include "replicated.hpp"
#include "serialization.hpp"
#include "signal.hpp"


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
        return ReadBlock(info);
    }


    void
    ReplicatedStorage::WriteBlock(BlockInfo info, BlockFormat data)
    {
        BlockFormat was = ReadBlock(info);
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
    ReplicatedNodeSet::AddNode(std::string address, short port)
    {
        parliament.AddLegislator(address, port);
        in_progress.Wait();
    }


    void
    ReplicatedNodeSet::RemoveNode(std::string address, short port)
    {
        parliament.RemoveLegislator(address, port);
        in_progress.Wait();
    }
}
