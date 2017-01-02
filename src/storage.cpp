#include <cstring>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

#include "commit.hpp"
#include "customhash.hpp"
#include "serialization.hpp"
#include "signal.hpp"
#include "storage.hpp"


namespace dafs
{
    ReplicatedStorage::ReplicatedStorage(
        Parliament& parliament,
        dafs::Signal& in_progress,
        Root root)
    : parliament(parliament),
      in_progress(in_progress),
      root(root)
    {
    }


    ReplicatedStorage::ReplicatedStorage(
        const ReplicatedStorage& other)
    : parliament(other.parliament),
      in_progress(other.in_progress),
      root(other.root)
    {
    }


    BlockFormat
    ReplicatedStorage::ReadBlock(BlockInfo info)
    {
        return ReadBlock(get_storage_info(info));
    }


    void
    ReplicatedStorage::WriteBlock(BlockInfo info, BlockFormat data)
    {
        BlockFormat was = ReadBlock(info);
        Delta delta = CreateDelta(info.path, was.contents, data.contents);

        do_write(get_storage_info(info), dafs::Serialize(delta));
    }


    void
    ReplicatedStorage::Write(BlockInfo info, Delta delta)
    {
        do_write(get_storage_info(info), dafs::Serialize(delta));
    }


    void
    ReplicatedStorage::do_write(
        BlockInfo info,
        std::string data)
    {
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


    dafs::BlockInfo
    ReplicatedStorage::get_storage_info(dafs::BlockInfo info)
    {
        info.path = (boost::filesystem::path(root.directory) /
                     boost::filesystem::path(info.path)).string();
        return info;
    }
}
