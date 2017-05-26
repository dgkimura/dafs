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
          identity(
              dafs::CreateBlockInfo(
                  boost::filesystem::path(Constant::IdentityName).string(),
                  dafs::Identity())),
          details(
              dafs::CreateBlockInfo(
                  boost::filesystem::path(Constant::DetailsName).string(),
                  dafs::Identity()))
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
          lock(other.lock),
          identity(other.identity)
    {
    }


    dafs::Identity
    ReplicatedPartition::GetIdentity()
    {
        auto id_block =  store.ReadBlock(identity);
        return dafs::Deserialize<dafs::Identity>(id_block.contents);
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


    void
    ReplicatedPartition::DeleteBlock(BlockInfo info)
    {
        store.DeleteBlock(info);
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
        std::string location)
    {
        dafs::ReplicatedEndpoints endpoints = GetNodeSetDetails();
        dafs::Address removed = endpoints.minus.replication;

        nodeset.SetMinus(management, replication, location, endpoints);
        WriteBlock(
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
        std::string location)
    {
        dafs::ReplicatedEndpoints endpoints = GetNodeSetDetails();

        nodeset.SetZero(management, replication, location, endpoints);
        WriteBlock(
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
        std::string location)
    {
        dafs::ReplicatedEndpoints endpoints = GetNodeSetDetails();
        dafs::Address removed = endpoints.plus.replication;

        nodeset.SetPlus(management, replication, location, endpoints);
        WriteBlock(
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
