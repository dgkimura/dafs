#include <boost/filesystem.hpp>

#include "commit.hpp"
#include "delta.hpp"
#include "disk.hpp"
#include "partition.hpp"
#include "serialization.hpp"


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
                  (boost::filesystem::path(root.directory) /
                   boost::filesystem::path(Constant::FileListName)).string(),
                  dafs::CreateLocation("N/A"))),
          blocks(
              dafs::CreateBlockInfo(
                  (boost::filesystem::path(root.directory) /
                   boost::filesystem::path(Constant::BlockListName)).string(),
                  dafs::CreateLocation("N/A"))),
          nodes(
              dafs::CreateBlockInfo(
                  (boost::filesystem::path(root.directory) /
                   boost::filesystem::path(Constant::NodeSetName)).string(),
                  dafs::CreateLocation("N/A"))),
          identity(
              dafs::CreateBlockInfo(
                  (boost::filesystem::path(root.directory) /
                   boost::filesystem::path(Constant::IdentityName)).string(),
                  dafs::CreateLocation("N/A"))),
          in_progress()
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
          in_progress()
    {
    }


    int
    Partition::GetIdentity()
    {
        dafs::BlockFormat b =  store.ReadBlock(identity);
        return dafs::Deserialize<int>(b.contents);
    }


    void
    Partition::SetIdentity(int number)
    {
        dafs::Delta delta = dafs::Set(identity, number);
        store.Write(identity, delta);
    }


    void
    Partition::CreateFile(FileInfo info)
    {
        Delta delta = dafs::Insert(files, info);
        store.Write(files, delta);
    }


    void
    Partition::DeleteFile(FileInfo info)
    {
        Delta delta = dafs::Remove(files, info);
        store.Write(files, delta);
    }


    void
    Partition::CreateBlock(BlockInfo info)
    {
        Delta delta = dafs::Insert(blocks, info);
        store.Write(blocks, delta);
    }


    void
    Partition::DeleteBlock(BlockInfo info)
    {
        Delta delta = dafs::Remove(nodes, info);
        store.Write(blocks, delta);
    }


    BlockFormat
    Partition::ReadBlock(BlockInfo block)
    {
        return store.ReadBlock(block);
    }


    void
    Partition::WriteBlock(BlockInfo block, BlockFormat format)
    {
        dafs::Delta delta = dafs::Set(block, format.contents);
        store.Write(block, delta);
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
}
