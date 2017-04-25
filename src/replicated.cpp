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
        std::shared_ptr<dafs::Signal> in_progress)
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
        in_progress->Wait();
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
        in_progress->Wait();
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
    ReplicatedNodeSet::RemoveNode(dafs::Address address)
    {
        parliament.RemoveLegislator(address.ip, address.port);
    }


    dafs::ReplicatedEndpoints
    ReplicatedNodeSet::SetMinus(
        dafs::Address management,
        dafs::Address replication,
        std::string location,
        dafs::ReplicatedEndpoints& details)
    {
        parliament.AddLegislator(replication.ip, replication.port, location);
        details.minus.management = management;
        details.minus.replication = replication;
        return details;
    }


    dafs::ReplicatedEndpoints
    ReplicatedNodeSet::SetZero(
        dafs::Address management,
        dafs::Address replication,
        std::string location,
        dafs::ReplicatedEndpoints& details)
    {
        parliament.AddLegislator(replication.ip, replication.port, location);
        details.zero.management = management;
        details.zero.replication = replication;
        return details;
    }


    dafs::ReplicatedEndpoints
    ReplicatedNodeSet::SetPlus(
        const dafs::Address management,
        const dafs::Address replication,
        const std::string location,
        dafs::ReplicatedEndpoints& details)
    {
        parliament.AddLegislator(replication.ip, replication.port, location);
        details.plus.management = management;
        details.plus.replication = replication;
        return details;
    }


    ReplicatedPing::ReplicatedPing(
        Parliament& parliament,
        std::chrono::seconds ping_interval,
        std::shared_ptr<Signal> in_progress)
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
            in_progress->Wait();
            std::this_thread::sleep_for(ping_interval);
        }
    }

    std::vector<dafs::Address>
    ReplicatedPing::NonresponsiveMembers(int last_elections)
    {
        auto nonresponsive = parliament.GetLegislators();
        for (auto i : parliament.GetAbsenteeBallots(last_elections))
        {
            nonresponsive = nonresponsive->Intersection(i.second);
        }
        std::vector<dafs::Address> nonresponsive_endpoints;
        for (auto r : *nonresponsive)
        {
            nonresponsive_endpoints.push_back(dafs::Address(r.hostname, r.port));
        }
        return nonresponsive_endpoints;
    }
}
