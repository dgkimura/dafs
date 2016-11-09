#include <cstring>

#include <boost/filesystem.hpp>

#include "customhash.hpp"
#include "serialization.hpp"
#include "storage.hpp"


namespace dafs
{
    ReplicatedStorage::ReplicatedStorage(
        Parliament parliament,
        std::string dirname,
        dafs::BlockInfo file_info_list,
        dafs::BlockInfo block_info_list):
    dirname(dirname),
    parliament(parliament),
    file_info_list(file_info_list),
    block_info_list(block_info_list)
    {
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
        std::fstream s((fs::path(dirname) / fs::path(info.name)).string(),
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
        std::fstream f((fs::path(dirname) / fs::path(info.filename)).string(),
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

        Delta delta = CreateDelta(info.filename, was.contents, is.contents);

        do_write(ProposalType::WriteDelta, info, dafs::Serialize(delta));
    }


    void
    ReplicatedStorage::WriteBlock(BlockInfo info, BlockFormat data)
    {
        BlockFormat was = ReadBlock(info);
        Delta delta = CreateDelta(info.filename, was.contents, data.contents);

        do_write(ProposalType::WriteDelta, info, dafs::Serialize(delta));
    }


    void
    ReplicatedStorage::do_write(
        dafs::ProposalType type,
        BlockInfo info,
        std::string data)
    {
        info.filename = (fs::path(dirname) / fs::path(info.filename)).string();
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
}
