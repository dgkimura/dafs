#include <boost/filesystem.hpp>

#include "dafs/commit.hpp"
#include "dafs/delta.hpp"
#include "dafs/disk.hpp"
#include "dafs/partition.hpp"
#include "dafs/serialization.hpp"


namespace dafs
{
    Partition::Partition(
        Root root
    )
        : parliament(root.directory,
                     DecreeHandler(dafs::Commit(parliament, in_progress))),
          store(parliament, in_progress),
          nodeset(parliament, in_progress),
          files(
              dafs::CreateBlockInfo(
                  boost::filesystem::path(Constant::FileListName).string(),
                  dafs::CreateLocation("N/A"))),
          blocks(
              dafs::CreateBlockInfo(
                  boost::filesystem::path(Constant::BlockListName).string(),
                  dafs::CreateLocation("N/A"))),
          nodes(
              dafs::CreateBlockInfo(
                  boost::filesystem::path(Constant::NodeSetName).string(),
                  dafs::CreateLocation("N/A"))),
          identity(
              dafs::CreateBlockInfo(
                  boost::filesystem::path(Constant::IdentityName).string(),
                  dafs::CreateLocation("N/A"))),
          in_progress(),
          root(root)
    {
    }


    Partition::Partition(
        const Partition& other
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


    dafs::Partition::Identity
    Partition::GetIdentity()
    {
        dafs::BlockFormat b =  store.ReadBlock(rooted(identity));
        return Identity
        {
            dafs::Deserialize<std::string>(b.contents)
        };
    }


    void
    Partition::SetIdentity(dafs::Partition::Identity id)
    {
        dafs::Delta delta = dafs::Set(rooted(identity), id.identity);
        store.Write(rooted(identity), delta);
    }


    void
    Partition::CreateFile(FileInfo info)
    {
        Delta delta = dafs::Insert(rooted(files), info);
        store.Write(rooted(files), delta);
    }


    void
    Partition::DeleteFile(FileInfo info)
    {
        Delta delta = dafs::Remove(rooted(files), info);
        store.Write(rooted(files), delta);
    }


    void
    Partition::CreateBlock(BlockInfo info)
    {
        Delta delta = dafs::Insert(rooted(blocks), info);
        store.Write(rooted(blocks), delta);
    }


    void
    Partition::DeleteBlock(BlockInfo info)
    {
        Delta delta = dafs::Remove(rooted(nodes), info);
        store.Write(rooted(blocks), delta);
    }


    BlockFormat
    Partition::ReadBlock(BlockInfo block)
    {
        return store.ReadBlock(rooted(block));
    }


    void
    Partition::WriteBlock(BlockInfo block, BlockFormat format)
    {
        dafs::Delta delta = dafs::Set(rooted(block), format.contents);
        store.Write(rooted(block), delta);
    }


    void
    Partition::AddNode(std::string address, short port)
    {
        nodeset.AddNode(address, port);
    }


    void
    Partition::RemoveNode(std::string address, short port)
    {
        nodeset.RemoveNode(address, port);
    }


    dafs::BlockInfo
    Partition::rooted(dafs::BlockInfo info)
    {
        info.path = (boost::filesystem::path(root.directory) /
                     boost::filesystem::path(info.path)).string();
        return info;
    }
}
