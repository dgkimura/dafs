#include <boost/filesystem.hpp>

#include "dafs/delta.hpp"
#include "dafs/disk.hpp"
#include "dafs/partition.hpp"
#include "dafs/serialization.hpp"


namespace dafs
{
    ReplicatedPartition::ReplicatedPartition(
        Address address,
        Root root,
        std::chrono::seconds ping_interval
    )
        : replication_(address, root),
          store(replication_, root),
          nodeset(replication_),
          ping(replication_,
               address,
               [this]()->dafs::ReplicatedEndpoints
               {
                   return GetNodeSetDetails();
               },
               [this]()->bool
               {
                   return lock.IsLocked();
               },
               ping_interval),
          lock(replication_, address, root),
          details(
              dafs::BlockInfo
              {
                  boost::filesystem::path(Constant::DetailsName).string(),
                  dafs::Identity()
              }
          )
    {
        ping.Start();
    }


    ReplicatedPartition::ReplicatedPartition(
        const ReplicatedPartition& other
    )
        : replication_(other.replication_),
          store(other.store),
          nodeset(other.nodeset),
          ping(other.ping),
          lock(other.lock)
    {
    }


    dafs::Identity
    ReplicatedPartition::GetIdentity()
    {
        auto details_block =  store.ReadBlock(details);
        auto details = dafs::Deserialize<dafs::ReplicatedEndpoints>(
            details_block.contents);
        return details.zero.identity;
    }


    dafs::ReplicatedEndpoints
    ReplicatedPartition::GetNodeSetDetails()
    {
        auto details_block =  store.ReadBlock(details);
        return dafs::Deserialize<dafs::ReplicatedEndpoints>(details_block.contents);
    }

    bool
    ReplicatedPartition::IsAddressResponsive(dafs::Address address)
    {
        for (auto r : ping.NonresponsiveMembers())
        {
            if (r.ip == address.ip && r.port == address.port)
            {
                return false;
            }
        }
        return true;
    }


    dafs::BlockInfo
    ReplicatedPartition::AllocateBlock()
    {
        dafs::BlockInfo info {"", GetNodeSetDetails().zero.identity, 0};

        while (IsLogicallyOrdered(GetNodeSetDetails().zero.identity,
                                  info.identity,
                                  GetNodeSetDetails().plus.identity))
        {
            auto index = store.GetIndex().items;
            if (std::any_of(index.cbegin(), index.cend(),
                            [=](const BlockInfo& b)
                            {
                                return b.identity==info.identity;
                            }))
            {
                info.identity += 1;
            }
            else
            {
                store.InsertIndex(info);
                break;
            }
        }
        return info;
    }


    void
    ReplicatedPartition::DeleteBlock(BlockInfo info)
    {
        store.DeleteBlock(info);
        store.RemoveIndex(info);
    }


    BlockFormat
    ReplicatedPartition::ReadBlock(BlockInfo block)
    {
        return store.ReadBlock(block);
    }


    void
    ReplicatedPartition::WriteBlock(BlockInfo block, BlockFormat format)
    {
        store.WriteBlock(block, format);

        auto index = store.GetIndex().items;
        if (!std::any_of(index.cbegin(), index.cend(),
                         [=](const BlockInfo& b)
                         {
                             return b.identity==block.identity;
                         }))
        {
            store.InsertIndex(block);
        }
    }


    BlockIndex
    ReplicatedPartition::GetIndex()
    {
        return store.GetIndex();
    }


    void
    ReplicatedPartition::SetMinus(
        dafs::Address management,
        dafs::Address replication,
        dafs::Identity identity,
        std::string location)
    {
        dafs::Address removed = GetNodeSetDetails().minus.replication;

        auto endpoints = nodeset.SetMinus(management,
                                          replication,
                                          identity,
                                          location,
                                          GetNodeSetDetails());
        store.WriteBlock(
            details,
            BlockFormat
            {
                dafs::Serialize(endpoints)
            }
        );
        if (removed.port != dafs::EmptyAddress().port)
        {
            nodeset.RemoveNode(removed);
        }
    }

    void
    ReplicatedPartition::SetZero(
        dafs::Address management,
        dafs::Address replication,
        dafs::Identity identity,
        std::string location)
    {
        auto endpoints = nodeset.SetZero(management,
                                         replication,
                                         identity,
                                         location,
                                         GetNodeSetDetails());
        store.WriteBlock(
            details,
            BlockFormat
            {
                dafs::Serialize(endpoints)
            }
        );
    }

    void
    ReplicatedPartition::SetPlus(
        dafs::Address management,
        dafs::Address replication,
        dafs::Identity identity,
        std::string location)
    {
        dafs::Address removed = GetNodeSetDetails().plus.replication;

        auto endpoints = nodeset.SetPlus(management,
                                         replication,
                                         identity,
                                         location,
                                         GetNodeSetDetails());

        store.WriteBlock(
            details,
            BlockFormat
            {
                dafs::Serialize(endpoints)
            }
        );
        if (removed.port != dafs::EmptyAddress().port)
        {
            nodeset.RemoveNode(removed);
        }
    }


    bool
    ReplicatedPartition::IsActive()
    {
        auto endpoints = GetNodeSetDetails();

        return endpoints.minus.replication.ip != dafs::EmptyAddress().ip &&
               endpoints.minus.replication.port != dafs::EmptyAddress().port &&
               endpoints.plus.replication.ip != dafs::EmptyAddress().ip &&
               endpoints.plus.replication.port != dafs::EmptyAddress().port;
    }


    bool
    ReplicatedPartition::Acquire()
    {
        return lock.Acquire();
    }


    void
    ReplicatedPartition::Release()
    {
        lock.Release();
    }
}
