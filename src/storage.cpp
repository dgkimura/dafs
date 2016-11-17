#include <cstring>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

#include "customhash.hpp"
#include "serialization.hpp"
#include "storage.hpp"


namespace dafs
{
    ReplicatedStorage::ReplicatedStorage(std::string directory)
        : directory(directory),
          parliament(directory)
    {
        file_info_list = dafs::CreateBlockInfo(
            (fs::path(directory) / fs::path("filelist")).string(),
            dafs::CreateLocation("localhost")
        );
        block_info_list = dafs::CreateBlockInfo(
            (fs::path(directory) / fs::path("blocklist")).string(),
            dafs::CreateLocation("localhost")
        );

        load_nodes();
    }

    void
    ReplicatedStorage::CreateFile(FileInfo info)
    {
        do_write(ProposalType::CreateFile,
                 file_info_list,
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
                 file_info_list,
                 dafs::Serialize(info));
    }


    FileFormat
    ReplicatedStorage::ReadFile(FileInfo info)
    {
        std::fstream s(fs::path(info.path).string(),
                        std::ios::out | std::ios::in | std::ios::binary);
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
                 block_info_list,
                 dafs::Serialize(info));
    }


    void
    ReplicatedStorage::DeleteBlock(BlockInfo info)
    {
        do_write(ProposalType::RemoveBlock,
                 block_info_list,
                 dafs::Serialize(info));
    }


    BlockFormat
    ReplicatedStorage::ReadBlock(BlockInfo info)
    {
        std::fstream f(fs::path(info.path).string(),
                        std::ios::out | std::ios::in | std::ios::binary);
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

        do_write(ProposalType::WriteDelta, info, dafs::Serialize(delta));
    }


    void
    ReplicatedStorage::WriteBlock(BlockInfo info, BlockFormat data)
    {
        BlockFormat was = ReadBlock(info);
        Delta delta = CreateDelta(info.path, was.contents, data.contents);

        do_write(ProposalType::WriteDelta, info, dafs::Serialize(delta));
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
                CreateBlockEditProposal
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
    ReplicatedStorage::load_nodes()
    {
        if (boost::filesystem::exists(
            fs::path(directory / fs::path("nodeset"))))
        {
            std::fstream s(fs::path(directory / fs::path("nodeset")).string(),
                           std::ios::in | std::ios::binary);
            for (auto node_string : dafs::Deserialize<NodeSet>(s).nodes)
            {
                std::vector<std::string> hostport;
                split(hostport, node_string, boost::is_any_of(":"));
                parliament.AddLegislator(
                    hostport[0],
                    boost::lexical_cast<short>(hostport[1]));
            }
        }
    }
}
