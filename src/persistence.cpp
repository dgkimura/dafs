#include <cstring>

#include <boost/filesystem.hpp>

#include "persistence.hpp"
#include "proposals.hpp"
#include "serialization.hpp"


namespace dafs
{
    Durable::Durable(Parliament parliament_, std::string dirname_)
        : parliament(parliament_),
          dirname(dirname_)
    {
    }


    BlockFormat
    Durable::Get(BlockInfo info)
    {
        std::fstream f((fs::path(dirname) / fs::path(info.filename)).string(),
                        std::ios::out | std::ios::in | std::ios::binary);
        BlockFormat b;
        f.read(b.contents, BLOCK_SIZE_IN_BYTES);
        return b;
    }


    void
    Durable::Write(BlockInfo info, Bytes data)
    {
        BlockFormat was = Get(info);
        BlockFormat is(was);

        std::memmove(is.contents + info.offset, data.contents,
                     BLOCK_SIZE_IN_BYTES - info.offset);

        Delta delta = CreateDelta(info.filename, was.contents, is.contents);

        do_write(ProposalType::BlockWriteDelta, info, dafs::Serialize(delta));
    }


    void
    Durable::Insert(BlockInfo info, FileInfo file)
    {
        do_write(ProposalType::SuperBlockInsert, info, dafs::Serialize(file));
    }


    void
    Durable::Remove(BlockInfo info, FileInfo file)
    {
        do_write(ProposalType::SuperBlockRemove, info, dafs::Serialize(file));
    }


    void
    Durable::do_write(dafs::ProposalType type, BlockInfo info, std::string data)
    {
        info.filename = (fs::path(dirname) / fs::path(info.filename)).string();
        //TODO: info.hash = ...
        parliament.CreateProposal
        (
            dafs::Serialize<dafs::Proposal>
            (
                CreateBlockEditProposal
                (
                    type,
                    data,
                    info
                )
            )
        );
    }
}
