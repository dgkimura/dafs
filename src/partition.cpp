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
        Root root
    )
        : parliament(Replica(address.ip, address.port),
                     root.directory,
                     DecreeHandler(dafs::Commit(parliament, in_progress))),
          store(parliament, in_progress),
          nodeset(parliament, in_progress),
          blocks(
              dafs::CreateBlockInfo(
                  boost::filesystem::path(Constant::BlockListName).string(),
                  dafs::Identity())),
          nodes(
              dafs::CreateBlockInfo(
                  boost::filesystem::path(Constant::NodeSetName).string(),
                  dafs::Identity())),
          identity(
              dafs::CreateBlockInfo(
                  boost::filesystem::path(Constant::IdentityName).string(),
                  dafs::Identity())),
          author(
              dafs::CreateBlockInfo(
                  boost::filesystem::path(Constant::AuthorName).string(),
                  dafs::Identity())),
          in_progress(),
          root(root),
          replication_interface(address)
    {
    }


    ReplicatedPartition::ReplicatedPartition(
        const ReplicatedPartition& other
    )
        : parliament(other.parliament),
          store(other.store),
          nodeset(other.nodeset),
          blocks(other.blocks),
          nodes(other.nodes),
          identity(other.identity),
          in_progress(),
          root(other.root),
          replication_interface(other.replication_interface)
    {
    }


    dafs::PartitionDetails
    ReplicatedPartition::GetDetails()
    {
        dafs::BlockFormat author_block =  store.ReadBlock(rooted(author));
        dafs::BlockFormat id_block =  store.ReadBlock(rooted(identity));

        return dafs::PartitionDetails
        {
            dafs::Deserialize<dafs::Address>(author_block.contents),
            replication_interface,
            dafs::Deserialize<dafs::Identity>(id_block.contents)
        };
    }


    void
    ReplicatedPartition::CreateBlock(BlockInfo info)
    {
        Delta delta = dafs::Insert(rooted(blocks), info);
        store.Write(rooted(blocks), delta);
    }


    void
    ReplicatedPartition::DeleteBlock(BlockInfo info)
    {
        store.DeleteBlock(rooted(info));

        Delta delta = dafs::Remove(rooted(blocks), info);
        store.Write(rooted(blocks), delta);
    }


    bool
    ReplicatedPartition::ContainsBlock(BlockInfo info)
    {
        return dafs::Contains(rooted(blocks), info);
    }


    BlockFormat
    ReplicatedPartition::ReadBlock(BlockInfo block)
    {
        return store.ReadBlock(rooted(block));
    }


    void
    ReplicatedPartition::WriteBlock(BlockInfo block, BlockFormat format)
    {
        dafs::Delta delta = dafs::Set(rooted(block), format.contents);
        store.Write(rooted(block), delta);
        if (!ContainsBlock(block))
        {
            CreateBlock(block);
        }
    }


    void
    ReplicatedPartition::AddNode(dafs::Address address)
    {
        nodeset.AddNode(address);
    }


    void
    ReplicatedPartition::RemoveNode(dafs::Address address)
    {
        nodeset.RemoveNode(address);
    }


    void
    ReplicatedPartition::Clear()
    {
        if (boost::filesystem::exists(root.directory))
        {
            for (auto& entry : boost::make_iterator_range(
                               boost::filesystem::directory_iterator(
                                   root.directory), {}))
            {
                std::remove(entry.path().string().c_str());
            }
        }
    }


    dafs::BlockInfo
    ReplicatedPartition::rooted(dafs::BlockInfo info)
    {
        info.path = (boost::filesystem::path(root.directory) /
                     boost::filesystem::path(info.path)).string();
        return info;
    }
}
