#include "proposals.hpp"
#include "serialization.hpp"


namespace dafs
{
    Proposal
    CreateBlockEditProposal(
        ProposalType type,
        std::string item,
        BlockInfo block)
    {
        BlockEdit edit;
        edit.filename = block.filename;
        edit.change = item;

        Proposal p;
        p.type = type;
        p.content = dafs::Serialize(edit);
        return p;
    }


    void
    GlobalProposalHandler(std::string proposal)
    {
        Proposal p = dafs::Deserialize<dafs::Proposal>(proposal);
        proposal_map.at(p.type)(p.content);
    }


    void
    ProposeCreateFile(std::string _edit)
    {
        dafs::BlockEdit edit = dafs::Deserialize<dafs::BlockEdit>(_edit);

        dafs::FileInfo file = dafs::Deserialize<dafs::FileInfo>(edit.change);

        std::fstream s(edit.filename,
                       std::ios::out | std::ios::in | std::ios::binary);
        dafs::BlockIndex index = dafs::Deserialize<dafs::BlockIndex>(s);

        // 1: check hash and revision of block info list
        // 2: add file to block info list with revision 0
    }


    void
    ProposeRemoveFile(std::string _edit)
    {
        dafs::BlockEdit edit = dafs::Deserialize<dafs::BlockEdit>(_edit);

        dafs::FileInfo file = dafs::Deserialize<dafs::FileInfo>(edit.change);

        // 1: check hash and revision of block info list
        // 2: delete file from block info list
    }


    void
    ProposeCreateBlock(std::string _edit)
    {
        dafs::BlockEdit edit = dafs::Deserialize<dafs::BlockEdit>(_edit);

        std::fstream s(edit.filename,
                       std::ios::out | std::ios::in | std::ios::binary);
        dafs::BlockIndex index = dafs::Deserialize<dafs::BlockIndex>(s);

        // 1. add block
        // 2. write out file
    }


    void
    ProposeRemoveBlock(std::string _edit)
    {
        dafs::BlockEdit edit = dafs::Deserialize<dafs::BlockEdit>(_edit);

        std::fstream s(edit.filename,
                       std::ios::out | std::ios::in | std::ios::binary);
        dafs::BlockIndex index = dafs::Deserialize<dafs::BlockIndex>(s);

        // 1. remove block
        // 2. write out file
    }


    void
    ProposeWriteDelta(std::string _edit)
    {
        dafs::BlockEdit edit = dafs::Deserialize<dafs::BlockEdit>(_edit);

        dafs::Delta delta = dafs::Deserialize<dafs::Delta>(edit.change);

        // 1: check hash
        // 2: update hash
        // 3: info.revision += 1
        // 4: write out info
    }
}
