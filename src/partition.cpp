#include <boost/filesystem.hpp>

#include "dafs/commit.hpp"
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
        : parliament(Replica(address.ip, address.port),
                     root.directory,
                     dafs::Commit(parliament, root, in_progress)),
          store(parliament, root, in_progress),
          nodeset(parliament),
          ping(parliament, ping_interval, in_progress),
          identity(
              dafs::CreateBlockInfo(
                  boost::filesystem::path(Constant::IdentityName).string(),
                  dafs::Identity())),
          author(
              dafs::CreateBlockInfo(
                  boost::filesystem::path(Constant::AuthorName).string(),
                  dafs::Identity())),
          in_progress(),
          replication_interface(address)
    {
    }


    ReplicatedPartition::ReplicatedPartition(
        const ReplicatedPartition& other
    )
        : parliament(other.parliament),
          store(other.store),
          nodeset(other.nodeset),
          ping(other.ping),
          identity(other.identity),
          in_progress(),
          replication_interface(other.replication_interface)
    {
    }


    dafs::PartitionDetails
    ReplicatedPartition::GetDetails()
    {
        auto author_block =  store.ReadBlock(author);
        auto id_block =  store.ReadBlock(identity);

        return dafs::PartitionDetails
        {
            dafs::Deserialize<dafs::Address>(author_block.contents),
            replication_interface,
            dafs::Deserialize<dafs::Identity>(id_block.contents)
        };
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
        if (!store.ContainsIndex(block))
        {
            store.InsertIndex(block);
        }
    }


    void
    ReplicatedPartition::AddNode(dafs::Address address, std::string location)
    {
        nodeset.AddNode(address, location);
    }


    void
    ReplicatedPartition::RemoveNode(dafs::Address address)
    {
        nodeset.RemoveNode(address);
    }

    std::vector<dafs::Address>
    ReplicatedPartition::NonresponsiveMembers(int last_elections)
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


    bool
    ReplicatedPartition::IsActive()
    {
        auto block =  store.ReadBlock(author);
        auto address = dafs::Deserialize<dafs::Address>(block.contents);

        return address.ip != dafs::EmptyAddress().ip &&
               address.port != dafs::EmptyAddress().port;
    }
}
