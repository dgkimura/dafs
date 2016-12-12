#include <cstring>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

#include "commit.hpp"
#include "customhash.hpp"
#include "serialization.hpp"
#include "serialization.hpp"
#include "storage.hpp"


namespace dafs
{
    ReplicatedStorage::ReplicatedStorage(
        std::string directory,
        std::string hostport,
        int identity)
    : parliament(directory, dafs::Commit(parliament, in_progress)),
      in_progress()
    {
        files_info = dafs::CreateBlockInfo(
            (fs::path(directory) / fs::path(Constant::FileListName)).string(),
            dafs::CreateLocation(hostport)
        );
        blocks_info = dafs::CreateBlockInfo(
            (fs::path(directory) / fs::path(Constant::BlockListName)).string(),
            dafs::CreateLocation(hostport)
        );
        nodeset_info = dafs::CreateBlockInfo(
            (fs::path(directory) / fs::path(Constant::NodeSetName)).string(),
            dafs::CreateLocation(hostport)
        );
        identity_info = dafs::CreateBlockInfo(
            (fs::path(directory) / fs::path(Constant::IdentityName)).string(),
            dafs::CreateLocation(hostport)
        );

        set_identity(identity);
        set_blocks({files_info, blocks_info, nodeset_info, identity_info});
        load_nodes(hostport);
    }


    ReplicatedStorage::ReplicatedStorage(
        const ReplicatedStorage& other)
    : in_progress()
    {
    }


    int
    ReplicatedStorage::GetIdentity()
    {
        int identity;
        std::fstream s(fs::path(identity_info.path).string(),
                       std::ios::in | std::ios::binary);
        s >> identity;
        return identity;
    }

    void
    ReplicatedStorage::CreateFile(FileInfo info)
    {
        Delta delta = Insert(files_info, info);
        do_write(files_info, dafs::Serialize(info));
    }


    void
    ReplicatedStorage::DeleteFile(FileInfo info)
    {
        do_write(files_info, dafs::Serialize(info));
    }


    void
    ReplicatedStorage::CreateBlock(BlockInfo info)
    {
        Delta delta = Insert(blocks_info, info);
        do_write(blocks_info, dafs::Serialize(info));
    }


    void
    ReplicatedStorage::DeleteBlock(BlockInfo info)
    {
        Delta delta = Remove(nodeset_info, info);
        do_write(blocks_info, dafs::Serialize(info));
    }


    BlockFormat
    ReplicatedStorage::ReadBlock(BlockInfo info)
    {
        return ReadBlock(info);
    }


    void
    ReplicatedStorage::WriteBlock(BlockInfo info, BlockFormat data)
    {
        BlockFormat was = ReadBlock(info);
        Delta delta = CreateDelta(info.path, was.contents, data.contents);

        do_write(info, dafs::Serialize(delta));
    }


    void
    ReplicatedStorage::AddNode(std::string address, short port)
    {
        std::string item(address + ":" + std::to_string(port));
        Delta delta = Insert(nodeset_info, item);

        do_write(nodeset_info, dafs::Serialize(delta));
    }


    void
    ReplicatedStorage::RemoveNode(std::string address, short port)
    {
        std::string item(address + ":" + std::to_string(port));
        Delta delta = Remove(nodeset_info, item);

        do_write(nodeset_info, dafs::Serialize(delta));
    }


    void
    ReplicatedStorage::do_write(
        BlockInfo info,
        std::string data)
    {
        info.path = fs::path(info.path).string();

        parliament.SendProposal
        (
            dafs::Serialize<dafs::Proposal>
            (
                CreateProposal
                (
                    dafs::ProposalType::WriteBlock,
                    data,
                    info,
                    info.revision
                )
            )
        );
        in_progress.Wait();
    }


    void
    ReplicatedStorage::load_nodes(
        std::string hostport)
    {
        if (boost::filesystem::exists(nodeset_info.path))
        {
            std::fstream f(fs::path(nodeset_info.path).string(),
                           std::ios::in | std::ios::binary);

            for (auto node_string : dafs::DeserializeFromBlockFormat<Index<std::string>>(f).items)
            {
                std::vector<std::string> hostport_;
                split(hostport_, node_string, boost::is_any_of(":"));
                parliament.AddLegislator(
                    hostport_[0],
                    boost::lexical_cast<short>(hostport_[1]));
            }
        }
        else
        {
            Index<std::string> nodeset;
            nodeset.items.push_back(hostport);

            std::fstream f(fs::path(nodeset_info.path).string(),
                           std::ios::out | std::ios::binary);
            f << dafs::SerializeIntoBlockFormat(nodeset);
            f.flush();
        }
    }


    void
    ReplicatedStorage::set_blocks(std::vector<dafs::BlockInfo> blocks)
    {
        if (!boost::filesystem::exists(blocks_info.path))
        {
            std::fstream f(fs::path(blocks_info.path).string(),
                           std::ios::out | std::ios::binary);

            dafs::BlockIndex index;
            index.items = blocks;
            f << dafs::Serialize(index);
            f.flush();
        }
    }


    void
    ReplicatedStorage::set_identity(int id)
    {
        std::fstream s(fs::path(identity_info.path).string(),
                       std::ios::out | std::ios::binary);
        s << id;
        s.flush();
    }
}
