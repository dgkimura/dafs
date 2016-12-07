#include <cstring>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

#include "customhash.hpp"
#include "proposals.hpp"
#include "serialization.hpp"
#include "serialization.hpp"
#include "storage.hpp"


namespace dafs
{
    ReplicatedStorage::ReplicatedStorage(
        std::string directory,
        std::string hostport,
        int identity)
    : parliament(directory, Action(parliament, in_progress)),
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
        do_write(ProposalType::CreateFile,
                 files_info,
                 dafs::Serialize(info));
    }


    void
    ReplicatedStorage::DeleteFile(FileInfo info)
    {
        do_write(ProposalType::RemoveFile,
                 files_info,
                 dafs::Serialize(info));
    }


    void
    ReplicatedStorage::CreateBlock(BlockInfo info)
    {
        BlockFormat block = ReadBlock(blocks_info);

        auto oldindex = dafs::Deserialize<BlockIndex>(block.contents);
        auto newindex = dafs::Deserialize<BlockIndex>(block.contents);
        newindex.blocks.push_back(info);

        Delta delta = CreateDelta(blocks_info.path,
                                  dafs::SerializeIntoBlockFormat(oldindex),
                                  dafs::SerializeIntoBlockFormat(newindex));
        do_write(ProposalType::CreateBlock,
                 blocks_info,
                 dafs::Serialize(info));
    }


    void
    ReplicatedStorage::DeleteBlock(BlockInfo info)
    {
        BlockFormat block = ReadBlock(blocks_info);

        auto oldindex = dafs::Deserialize<BlockIndex>(block.contents);
        auto newindex = dafs::Deserialize<BlockIndex>(block.contents);
        newindex.blocks.erase
        (
            std::remove_if
            (
                newindex.blocks.begin(),
                newindex.blocks.end(),
                [=](const BlockInfo& current)
                {
                    return current.path == info.path &&
                           current.revision == info.revision;
                }
            ),
            newindex.blocks.end()
        );

        Delta delta = CreateDelta(blocks_info.path,
                                  dafs::SerializeIntoBlockFormat(oldindex),
                                  dafs::SerializeIntoBlockFormat(newindex));
        do_write(ProposalType::RemoveBlock,
                 blocks_info,
                 dafs::Serialize(info));
    }


    BlockFormat
    ReplicatedStorage::ReadBlock(BlockInfo info)
    {
        std::fstream f(fs::path(info.path).string(),
                       std::ios::in | std::ios::binary);
        BlockFormat b = dafs::Deserialize<BlockFormat>(f);
        f.close();
        return b;
    }


    void
    ReplicatedStorage::WriteBlock(BlockInfo info, BlockFormat data)
    {
        BlockFormat was = ReadBlock(info);
        Delta delta = CreateDelta(info.path, was.contents, data.contents);

        do_write(ProposalType::WriteDelta,
                 info,
                 dafs::Serialize(delta));
    }


    void
    ReplicatedStorage::AddNode(std::string address, short port)
    {
        BlockFormat block = ReadBlock(nodeset_info);

        auto oldset = dafs::Deserialize<NodeSet>(block.contents);
        auto newset = dafs::Deserialize<NodeSet>(block.contents);
        newset.endpoints.push_back(address + ":" + std::to_string(port));

        Delta delta = CreateDelta(nodeset_info.path,
                                  dafs::SerializeIntoBlockFormat(oldset),
                                  dafs::SerializeIntoBlockFormat(newset));
        do_write(ProposalType::AddNode,
                 nodeset_info,
                 dafs::Serialize(delta));
    }


    void
    ReplicatedStorage::RemoveNode(std::string address, short port)
    {
        BlockFormat block = ReadBlock(nodeset_info);

        auto oldset = dafs::Deserialize<NodeSet>(block.contents);
        auto newset = dafs::Deserialize<NodeSet>(block.contents);
        newset.endpoints.erase
        (
            std::remove_if
            (
                newset.endpoints.begin(),
                newset.endpoints.end(),
                [=](const std::string& current)
                {
                    return current == address + ":" + std::to_string(port);
                }
            ),
            newset.endpoints.end()
        );

        Delta delta = CreateDelta(nodeset_info.path,
                                  dafs::SerializeIntoBlockFormat(oldset),
                                  dafs::SerializeIntoBlockFormat(newset));

        do_write(ProposalType::RemoveNode,
                 nodeset_info,
                 dafs::Serialize(delta));
    }


    void
    ReplicatedStorage::do_write(
        dafs::ProposalType type,
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
                    type,
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

            for (auto node_string : dafs::DeserializeFromBlockFormat<NodeSet>(f).endpoints)
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
            NodeSet nodeset;
            nodeset.endpoints.push_back(hostport);

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
            index.blocks = blocks;
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
