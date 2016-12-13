#include <boost/filesystem.hpp>

#include "delta.hpp"
#include "disk.hpp"
#include "partition.hpp"
#include "serialization.hpp"


namespace dafs
{
    Partition::Partition(
        dafs::Storage& store,
        dafs::NodeSet& nodeset,
        dafs::BlockInfo files,
        dafs::BlockInfo blocks,
        dafs::BlockInfo nodes,
        dafs::BlockInfo identity
    )
    : store(store),
      nodeset(nodeset),
      files(files),
      blocks(blocks),
      nodes(nodes),
      identity(identity)
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
        std::string item(address + ":" + std::to_string(port));
        Delta delta = dafs::Insert(nodes, item);

        store.Write(nodes, delta);
        nodeset.AddNode(address, port);
    }


    void
    Partition::RemoveNode(std::string address, short port)
    {
        std::string item(address + ":" + std::to_string(port));
        Delta delta = dafs::Remove(nodes, item);

        store.Write(nodes, delta);
        nodeset.RemoveNode(address, port);
    }
}
