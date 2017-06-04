#include <chrono>
#include <cstring>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

#include "dafs/constants.hpp"
#include "dafs/commit.hpp"
#include "dafs/customhash.hpp"
#include "dafs/details.hpp"
#include "dafs/disk.hpp"
#include "dafs/components.hpp"
#include "dafs/serialization.hpp"


namespace dafs
{
    ReplicatedStorage::ReplicatedStorage(
        dafs::Replication& replication,
        dafs::Root root)
    : replication(replication),
      root(root),
      blocks(
          dafs::BlockInfo
          {
              boost::filesystem::path(Constant::BlockListName).string(),
              dafs::Identity()
          }
      )
    {
    }


    ReplicatedStorage::ReplicatedStorage(
        const ReplicatedStorage& other)
    : replication(other.replication),
      root(other.root),
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


    BlockIndex
    ReplicatedStorage::GetIndex()
    {
        auto block = ReadBlock(blocks);

        return block.contents.empty() ? BlockIndex() :
               dafs::Deserialize<BlockIndex>(block.contents);
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
        const dafs::Address management,
        const dafs::Address replication,
        const dafs::Identity identity,
        const std::string location,
        dafs::ReplicatedEndpoints details)
    {
        replication_.AddReplica(replication, location);
        details.minus.management = management;
        details.minus.replication = replication;
        details.minus.identity = identity;
        return details;
    }


    dafs::ReplicatedEndpoints
    ReplicatedNodeSet::SetZero(
        const dafs::Address management,
        const dafs::Address replication,
        const dafs::Identity identity,
        const std::string location,
        dafs::ReplicatedEndpoints details)
    {
        replication_.AddReplica(replication, location);
        details.zero.management = management;
        details.zero.replication = replication;
        details.zero.identity = identity;
        return details;
    }


    dafs::ReplicatedEndpoints
    ReplicatedNodeSet::SetPlus(
        const dafs::Address management,
        const dafs::Address replication,
        const dafs::Identity identity,
        const std::string location,
        dafs::ReplicatedEndpoints details)
    {
        replication_.AddReplica(replication, location);
        details.plus.management = management;
        details.plus.replication = replication;
        details.plus.identity = identity;
        return details;
    }


    ReplicatedPing::ReplicatedPing(
        dafs::Replication& replication,
        dafs::Address address,
        std::function<dafs::ReplicatedEndpoints(void)> get_endpoints,
        std::function<bool(void)> is_partition_locked,
        std::chrono::seconds ping_interval)
    : replication(replication),
      address_(address),
      get_endpoints(get_endpoints),
      is_partition_locked(is_partition_locked),
      ping_interval(ping_interval),
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
                if (IsReplicatedPartition(get_endpoints()) && !is_partition_locked())
                {
                    SendPing(sender);
                }
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
        return replication.GetMissingReplicas();
    }


    dafs::Endpoint
    ReplicatedPing::get_failover_endpoint(dafs::Address inactive)
    {
        auto endpoints = get_endpoints();

        return GetFailover(endpoints, address_, inactive);
    }


    ReplicatedLock::ReplicatedLock(
        dafs::Replication& replication,
        dafs::Address address,
        dafs::Root root)
    : replication(replication),
      address(address),
      root(root)
    {
    }


    bool
    ReplicatedLock::Acquire()
    {
        dafs::BlockInfo lockfile;
        lockfile.path = Constant::LockName;

        replication.Write
        (
            dafs::Serialize<dafs::Proposal>
            (
                CreateProposal
                (
                    dafs::ProposalType::WriteBlock,
                    dafs::Serialize
                    (
                        CreateDelta(
                            rooted(lockfile).path,
                            "", // was empty
                            dafs::Serialize(address)
                        )
                    ),
                    lockfile,
                    0, // revision
                    0  // Write IFF was empty
                )
            )
        );

        return dafs::ReadBlock(rooted(lockfile)).contents == dafs::Serialize(address);
    }


    bool
    ReplicatedLock::IsLocked()
    {
        dafs::BlockInfo lockfile;
        lockfile.path = Constant::LockName;

        return dafs::ReadBlock(rooted(lockfile)).contents.size() > 0;
    }


    void
    ReplicatedLock::Release()
    {
        dafs::BlockInfo lockfile;
        lockfile.path = Constant::LockName;

        replication.Write
        (
            dafs::Serialize<dafs::Proposal>
            (
                CreateProposal
                (
                    dafs::ProposalType::DeleteBlock,
                    "",
                    lockfile,
                    lockfile.revision,
                    std::hash<dafs::BlockInfo>{}(lockfile)
                )
            )
        );
    }


    BlockInfo
    ReplicatedLock::rooted(
        BlockInfo info)
    {
        info.path = (boost::filesystem::path(root.directory) /
                     boost::filesystem::path(info.path)).string();
        return info;
    }
}
