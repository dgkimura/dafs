#include <cstring>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

#include "customhash.hpp"
#include "proposals.hpp"
#include "serialization.hpp"
#include "storage.hpp"


namespace dafs
{
    ReplicatedStorage::ReplicatedStorage(
        std::string directory,
        std::string hostport,
        int identity)
    : parliament(directory, Action(parliament))
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
        load_nodes(hostport);
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
    ReplicatedStorage::OpenFile(FileInfo info)
    {
    }


    void
    ReplicatedStorage::DeleteFile(FileInfo info)
    {
        do_write(ProposalType::RemoveFile,
                 files_info,
                 dafs::Serialize(info));
    }


    FileFormat
    ReplicatedStorage::ReadFile(FileInfo info)
    {
        std::fstream s(fs::path(info.path).string(),
                       std::ios::out | std::ios::binary);
        FileFormat f = dafs::Deserialize<FileFormat>(s);
        return f;
    }


    void
    ReplicatedStorage::WriteFile(FileInfo info, Bytes data)
    {
    }


    void
    ReplicatedStorage::CreateBlock(BlockInfo info)
    {
        do_write(ProposalType::CreateBlock,
                 blocks_info,
                 dafs::Serialize(info));
    }


    void
    ReplicatedStorage::DeleteBlock(BlockInfo info)
    {
        do_write(ProposalType::RemoveBlock,
                 blocks_info,
                 dafs::Serialize(info));
    }


    BlockFormat
    ReplicatedStorage::ReadBlock(BlockInfo info)
    {
        std::fstream f(fs::path(info.path).string(),
                       std::ios::out | std::ios::binary);
        BlockFormat b;
        f.read(b.contents, BLOCK_SIZE_IN_BYTES);
        return b;
    }


    void
    ReplicatedStorage::WriteBlock(BlockInfo info, Bytes data)
    {
        // TODO: Handle overflows to another block.
        BlockFormat was = ReadBlock(info);
        BlockFormat is(was);

        std::memmove(is.contents + info.offset, data.contents,
                     BLOCK_SIZE_IN_BYTES - info.offset);

        Delta delta = CreateDelta(info.path, was.contents, is.contents);

        do_write(ProposalType::WriteDelta,
                 info,
                 dafs::Serialize(delta));
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
        BlockFormat was = ReadBlock(nodeset_info);
        BlockFormat is(was);

        auto nodeset = dafs::Deserialize<NodeSet>(was.contents);
        nodeset.endpoints.push_back(address + std::to_string(port));
        std::memmove(is.contents, dafs::Serialize(nodeset).c_str(),
                     BLOCK_SIZE_IN_BYTES);

        Delta delta = CreateDelta(nodeset_info.path, was.contents,
                                  is.contents);

        do_write(ProposalType::AddNode,
                 nodeset_info,
                 dafs::Serialize(delta));
    }


    void
    ReplicatedStorage::RemoveNode(std::string address, short port)
    {
        BlockFormat was = ReadBlock(nodeset_info);
        BlockFormat is(was);

        auto nodeset = dafs::Deserialize<NodeSet>(was.contents);
        std::memmove(is.contents, dafs::Serialize(nodeset).c_str(),
                     BLOCK_SIZE_IN_BYTES);
        nodeset.endpoints.erase
        (
            std::remove_if
            (
                nodeset.endpoints.begin(),
                nodeset.endpoints.end(),
                [=](const std::string& current)
                {
                    return current == address + std::to_string(port);
                }
            ),
            nodeset.endpoints.end()
        );

        Delta delta = CreateDelta(nodeset_info.path, was.contents,
                                  is.contents);

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
    }


    void
    ReplicatedStorage::load_nodes(
        std::string hostport)
    {
        if (boost::filesystem::exists(nodeset_info.path))
        {
            std::fstream f(fs::path(nodeset_info.path).string(),
                           std::ios::in | std::ios::binary);

            for (auto node_string : dafs::Deserialize<NodeSet>(f).endpoints)
            {
                std::vector<std::string> hostport;
                split(hostport, node_string, boost::is_any_of(":"));
                parliament.AddLegislator(
                    hostport[0],
                    boost::lexical_cast<short>(hostport[1]));
            }
        }
        else
        {
            NodeSet nodeset;
            nodeset.endpoints.push_back(hostport);

            std::fstream f(fs::path(nodeset_info.path).string(),
                           std::ios::out | std::ios::binary);
            f << dafs::Serialize(nodeset);
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
