#include <chrono>
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
        dafs::Root root,
        dafs::Signal& in_progress)
    : parliament(parliament),
      root(root),
      in_progress(in_progress),
      blocks(
          dafs::CreateBlockInfo(
              boost::filesystem::path(Constant::BlockListName).string(),
              dafs::Identity())),
      nodes(
          dafs::CreateBlockInfo(
              boost::filesystem::path(Constant::NodeSetName).string(),
              dafs::Identity()))
    {
    }


    ReplicatedStorage::ReplicatedStorage(
        const ReplicatedStorage& other)
    : parliament(other.parliament),
      root(other.root),
      in_progress(other.in_progress),
      blocks(other.blocks),
      nodes(other.nodes)
    {
    }


    BlockFormat
    ReplicatedStorage::ReadBlock(BlockInfo info)
    {
        return dafs::ReadBlock(rooted(info));
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
                    info.revision,
                    std::hash<dafs::BlockInfo>{}(rooted(info))
                )
            )
        );
        in_progress.Wait();
    }


    void
    ReplicatedStorage::InsertIndex(BlockInfo info)
    {
        Delta delta = dafs::Insert(rooted(blocks), info);
        delta.filename = blocks.path;
        Write(blocks, delta);
    }


    void
    ReplicatedStorage::RemoveIndex(BlockInfo info)
    {
        Delta delta = dafs::Remove(rooted(blocks), info);
        delta.filename = blocks.path;
        Write(blocks, delta);
    }


    bool
    ReplicatedStorage::ContainsIndex(BlockInfo info)
    {
        return dafs::Contains(rooted(blocks), info);
    }


    void
    ReplicatedStorage::WriteBlock(BlockInfo info, BlockFormat data)
    {
        BlockFormat was = dafs::ReadBlock(rooted(info));
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
                    info.revision,
                    std::hash<dafs::BlockInfo>{}(rooted(info))
                )
            )
        );
        in_progress.Wait();
    }


    BlockInfo
    ReplicatedStorage::rooted(
        BlockInfo info)
    {
        info.path = (boost::filesystem::path(root.directory) /
                     boost::filesystem::path(info.path)).string();
        return info;
    }


    ReplicatedNodeSet::ReplicatedNodeSet(
        Parliament& parliament)
    : parliament(parliament)
    {
    }


    void
    ReplicatedNodeSet::AddNode(dafs::Address address, std::string location)
    {
        parliament.AddLegislator(address.ip, address.port, location);
    }


    void
    ReplicatedNodeSet::RemoveNode(dafs::Address address)
    {
        parliament.RemoveLegislator(address.ip, address.port);
    }


    ReplicatedPing::ReplicatedPing(
        Parliament& parliament,
        std::chrono::seconds ping_interval,
        Signal& in_progress)
    : parliament(parliament),
      ping_interval(ping_interval),
      in_progress(in_progress),
      should_continue(true)
    {
        std::thread([this]() { send_ping(); }).detach();
    }


    void
    ReplicatedPing::send_ping()
    {
        while (should_continue)
        {
            parliament.SendProposal
            (
                dafs::Serialize<dafs::Proposal>
                (
                    Proposal
                    {
                        dafs::ProposalType::Ping,
                        dafs::Serialize(dafs::ProposalContent{})
                    }
                )
            );
            in_progress.Wait();
            std::this_thread::sleep_for(ping_interval);
        }
    }
}
