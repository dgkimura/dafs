#include <boost/filesystem.hpp>

#include "dafs/delta.hpp"
#include "dafs/disk.hpp"
#include "dafs/partition.hpp"
#include "dafs/serialization.hpp"


namespace dafs
{
    ReplicatedPartition::ReplicatedPartition(
        std::unique_ptr<Replication> replication,
        std::shared_ptr<Storage> store_,
        std::unique_ptr<NodeSet> nodeset_,
        std::unique_ptr<Ping> ping_,
        std::shared_ptr<Lock> lock_
    )
        : replication(std::move(replication)),
          store(std::move(store_)),
          nodeset(std::move(nodeset_)),
          ping(std::move(ping_)),
          lock(std::move(lock_)),
          details(
              dafs::BlockInfo
              {
                  boost::filesystem::path(Constant::DetailsName).string(),
                  dafs::Identity()
              }
          ),
          allocator(
              [this]()->dafs::BlockIndex { return store->GetIndex(); },
              [this](dafs::BlockInfo info) { store->InsertIndex(info); },
              [this]()->dafs::ReplicatedEndpoints { return GetNodeSetDetails(); }
          )
    {
        ping->Start();
    }


    std::unique_ptr<dafs::ReplicatedPartition>
    ReplicatedPartition::Create(
        Address address,
        Root root,
        std::chrono::seconds ping_interval
    )
    {
        auto replication = std::unique_ptr<dafs::Replication>(
            new dafs::PaxosReplication(address, root));
        auto store = std::shared_ptr<dafs::Storage>(
            new dafs::ReplicatedStorage(*replication, root));
        auto nodeset = std::unique_ptr<dafs::NodeSet>(
            new dafs::ReplicatedNodeSet(*replication));
        auto lock = std::shared_ptr<dafs::Lock>(
            new dafs::ReplicatedLock(*replication, address, root));
        auto ping = std::unique_ptr<dafs::Ping>(
            new ReplicatedPing(
                *replication,
                address,
                [store]()
                {
                    auto details_block =  store->ReadBlock(
                        dafs::BlockInfo
                        {
                            boost::filesystem::path(Constant::DetailsName).string(),
                            dafs::Identity()
                        });
                    return dafs::Deserialize<dafs::ReplicatedEndpoints>(details_block.contents);
                },
                [lock]() { return lock->IsLocked(); },
                ping_interval));

        return std::unique_ptr<dafs::ReplicatedPartition>(
            new dafs::ReplicatedPartition(
                std::move(replication),
                std::move(store),
                std::move(nodeset),
                std::move(ping),
                std::move(lock)));
    }


    dafs::Identity
    ReplicatedPartition::GetIdentity()
    {
        auto details_block =  store->ReadBlock(details);
        auto details = dafs::Deserialize<dafs::ReplicatedEndpoints>(
            details_block.contents);
        return details.zero.identity;
    }


    dafs::ReplicatedEndpoints
    ReplicatedPartition::GetNodeSetDetails()
    {
        auto details_block =  store->ReadBlock(details);
        return dafs::Deserialize<dafs::ReplicatedEndpoints>(details_block.contents);
    }

    bool
    ReplicatedPartition::IsAddressResponsive(dafs::Address address)
    {
        for (auto r : ping->NonresponsiveMembers())
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
        return allocator.Allocate();
    }


    void
    ReplicatedPartition::DeleteBlock(BlockInfo info)
    {
        store->DeleteBlock(info);
        store->RemoveIndex(info);
    }


    BlockFormat
    ReplicatedPartition::ReadBlock(BlockInfo block)
    {
        return store->ReadBlock(block);
    }


    void
    ReplicatedPartition::WriteBlock(BlockInfo block, BlockFormat format)
    {
        store->WriteBlock(block, format);

        auto index = store->GetIndex().items;
        if (!std::any_of(index.cbegin(), index.cend(),
                         [=](const BlockInfo& b)
                         {
                             return b.identity==block.identity;
                         }))
        {
            store->InsertIndex(block);
        }
    }


    BlockIndex
    ReplicatedPartition::GetIndex()
    {
        return store->GetIndex();
    }


    void
    ReplicatedPartition::SetMinus(
        dafs::Address management,
        dafs::Address replication,
        dafs::Identity identity,
        std::string location)
    {
        dafs::Address removed = GetNodeSetDetails().minus.replication;

        auto endpoints = nodeset->SetMinus(management,
                                           replication,
                                           identity,
                                           location,
                                           GetNodeSetDetails());
        store->WriteBlock(
            details,
            BlockFormat
            {
                dafs::Serialize(endpoints)
            }
        );
        if (removed.port != dafs::EmptyAddress().port)
        {
            nodeset->RemoveNode(removed);
        }
    }

    void
    ReplicatedPartition::SetZero(
        dafs::Address management,
        dafs::Address replication,
        dafs::Identity identity,
        std::string location)
    {
        auto endpoints = nodeset->SetZero(management,
                                          replication,
                                          identity,
                                          location,
                                          GetNodeSetDetails());
        store->WriteBlock(
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

        auto endpoints = nodeset->SetPlus(management,
                                          replication,
                                          identity,
                                          location,
                                          GetNodeSetDetails());

        store->WriteBlock(
            details,
            BlockFormat
            {
                dafs::Serialize(endpoints)
            }
        );
        if (removed.port != dafs::EmptyAddress().port)
        {
            nodeset->RemoveNode(removed);
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
        return lock->Acquire();
    }


    void
    ReplicatedPartition::Release()
    {
        lock->Release();
    }
}
