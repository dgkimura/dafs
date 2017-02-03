#include <boost/filesystem.hpp>

#include "dafs/commit.hpp"
#include "dafs/delta.hpp"
#include "dafs/disk.hpp"
#include "dafs/partition.hpp"
#include "dafs/serialization.hpp"


namespace dafs
{
    ReplicatedPartition::ReplicatedPartition(
        Root root
    )
        : parliament(root.directory,
                     DecreeHandler(dafs::Commit(parliament, in_progress))),
          store(parliament, in_progress),
          nodeset(parliament, in_progress),
          files(
              dafs::CreateBlockInfo(
                  boost::filesystem::path(Constant::FileListName).string(),
                  dafs::Identity())),
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
          in_progress(),
          root(root)
    {
    }


    ReplicatedPartition::ReplicatedPartition(
        const ReplicatedPartition& other
    )
        : parliament(other.parliament),
          store(other.store),
          nodeset(other.nodeset),
          files(other.files),
          blocks(other.blocks),
          nodes(other.nodes),
          identity(other.identity),
          in_progress(),
          root(other.root)
    {
    }


    dafs::Identity
    ReplicatedPartition::GetIdentity()
    {
        dafs::BlockFormat b =  store.ReadBlock(rooted(identity));
        return dafs::Identity(b.contents);
    }


    void
    ReplicatedPartition::SetIdentity(dafs::Identity id)
    {
        dafs::Delta delta = dafs::Set(rooted(identity), id.id);
        store.Write(rooted(identity), delta);
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
        Delta delta = dafs::Remove(rooted(nodes), info);
        store.Write(rooted(blocks), delta);
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
    }


    void
    ReplicatedPartition::AddNode(std::string address, short port)
    {
        nodeset.AddNode(address, port);
    }


    void
    ReplicatedPartition::RemoveNode(std::string address, short port)
    {
        nodeset.RemoveNode(address, port);
    }


    dafs::BlockInfo
    ReplicatedPartition::rooted(dafs::BlockInfo info)
    {
        info.path = (boost::filesystem::path(root.directory) /
                     boost::filesystem::path(info.path)).string();
        return info;
    }
}
