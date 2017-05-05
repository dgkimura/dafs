#include <chrono>
#include <cstring>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

#include "dafs/commit.hpp"
#include "dafs/customhash.hpp"
#include "dafs/disk.hpp"
#include "dafs/components.hpp"
#include "dafs/serialization.hpp"
#include "dafs/signal.hpp"


namespace dafs
{
    ReplicatedStorage::ReplicatedStorage(
        dafs::Replication& replication,
        dafs::Root root,
        std::shared_ptr<dafs::Signal> in_progress)
    : replication(replication),
      root(root),
      in_progress(in_progress),
      blocks(
          dafs::CreateBlockInfo(
              boost::filesystem::path(Constant::BlockListName).string(),
              dafs::Identity()))
    {
    }


    ReplicatedStorage::ReplicatedStorage(
        const ReplicatedStorage& other)
    : replication(other.replication),
      root(other.root),
      in_progress(other.in_progress),
      blocks(other.blocks)
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
        replication.Write
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
        replication.Write
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
        dafs::Replication& replication)
    : replication_(replication)
    {
    }


    void
    ReplicatedNodeSet::RemoveNode(dafs::Address address)
    {
        replication_.RemoveReplica(address);
    }


    dafs::ReplicatedEndpoints
    ReplicatedNodeSet::SetMinus(
        dafs::Address management,
        dafs::Address replication,
        std::string location,
        dafs::ReplicatedEndpoints& details)
    {
        replication_.AddReplica(replication, location);
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
        replication_.AddReplica(replication, location);
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
        replication_.AddReplica(replication, location);
        details.plus.management = management;
        details.plus.replication = replication;
        return details;
    }


    ReplicatedPing::ReplicatedPing(
        dafs::Replication& replication,
        dafs::Address address,
        std::function<dafs::ReplicatedEndpoints(void)> get_endpoints,
        std::chrono::seconds ping_interval,
        std::shared_ptr<Signal> in_progress)
    : replication(replication),
      address_(address),
      get_endpoints(get_endpoints),
      ping_interval(ping_interval),
      in_progress(in_progress),
      should_continue(true)
    {
    }

    void
    ReplicatedPing::Start()
    {
        std::thread([this]()
        {
            auto sender = std::make_shared<dafs::NetworkSender>();
            while (should_continue)
            {
                SendPing(sender);
                std::this_thread::sleep_for(ping_interval);
            }
        }).detach();
    }


    void
    ReplicatedPing::SendPing(std::shared_ptr<dafs::Sender> sender)
    {
        replication.Write
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

        for (auto a : replication.GetMissingReplicas())
        {
            auto endpoint = get_failover_endpoint(a);
            if (endpoint.replication.ip == dafs::EmptyAddress().ip &&
                endpoint.replication.port == dafs::EmptyAddress().port)
            {
                break;
            }
            sender->Send(
                dafs::Message
                {
                    dafs::EmptyAddress(),
                    endpoint.management,
                    dafs::MessageType::_ProposeExitCluster,
                    std::vector<dafs::MetaData>
                    {
                        dafs::MetaData
                        {
                            dafs::AddressKey,
                            dafs::Serialize(a)
                        }
                    }
                }
            );
            break;
        }
    }

    std::vector<dafs::Address>
    ReplicatedPing::NonresponsiveMembers(int last_elections)
    {
        std::vector<dafs::Address> nonresponsive_endpoints;
        return nonresponsive_endpoints;
    }

    dafs::Endpoint
    ReplicatedPing::get_failover_endpoint(dafs::Address address)
    {
        auto endpoints = get_endpoints();

        if (endpoints.minus.replication.ip != address.ip &&
            endpoints.minus.replication.port != address.port &&
            endpoints.minus.replication.ip != address_.ip &&
            endpoints.minus.replication.port != address_.port)
        {
            return endpoints.minus;
        }
        if (endpoints.zero.replication.ip != address.ip &&
            endpoints.zero.replication.port != address.port &&
            endpoints.zero.replication.ip != address_.ip &&
            endpoints.zero.replication.port != address_.port)
        {
            return endpoints.zero;
        }
        if (endpoints.plus.replication.ip != address.ip &&
            endpoints.plus.replication.port != address.port &&
            endpoints.plus.replication.ip != address_.ip &&
            endpoints.plus.replication.port != address_.port)
        {
            return endpoints.plus;
        }
        return {
            dafs::EmptyAddress(),
            dafs::EmptyAddress()
        };
    }
}
