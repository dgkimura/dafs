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
        edit.info = block;
        edit.item = item;

        Proposal p;
        p.type = type;
        p.content = dafs::Serialize(edit);
        return p;
    }


    Proposer::Proposer(dafs::Storage& store_)
        : store(store_),
          proposal_map {
              {ProposalType::SuperBlockInsert, ProposeCreateFile},
              {ProposalType::SuperBlockRemove, ProposeRemoveFile},
              {ProposalType::BlockWriteDelta, ProposeWriteDelta}
          }
    {
    }


    void
    Proposer::operator()(std::string proposal)
    {
        Proposal p = dafs::Deserialize<dafs::Proposal>(proposal);
        proposal_map.at(p.type)(p.content, store);
    }


    void
    ProposeCreateFile(std::string _edit, dafs::Storage& store)
    {
        dafs::BlockEdit edit = dafs::Deserialize<dafs::BlockEdit>(_edit);

        dafs::BlockInfo info = edit.info;
        dafs::FileInfo file = dafs::Deserialize<dafs::FileInfo>(edit.item);

        // 1: check hash and revision of superblock
        // 2: add file to superblock with revision 0
    }


    void
    ProposeRemoveFile(std::string _edit, dafs::Storage& store)
    {
        dafs::BlockEdit edit = dafs::Deserialize<dafs::BlockEdit>(_edit);

        dafs::BlockInfo info = edit.info;
        dafs::FileInfo file = dafs::Deserialize<dafs::FileInfo>(edit.item);

        // 1: check hash and revision of superblock
        // 2: delete file from superblock
    }


    void
    ProposeCreateBlock(std::string _edit, dafs::Storage& store)
    {
        dafs::BlockEdit edit = dafs::Deserialize<dafs::BlockEdit>(_edit);

        dafs::BlockInfo info = edit.info;
        store.CreateBlock(info);
    }


    void
    ProposeRemoveBlock(std::string _edit, dafs::Storage& store)
    {
        dafs::BlockEdit edit = dafs::Deserialize<dafs::BlockEdit>(_edit);

        dafs::BlockInfo info = edit.info;
        store.DeleteBlock(info);
    }


    void
    ProposeWriteDelta(std::string _edit, dafs::Storage& store)
    {
        dafs::BlockEdit edit = dafs::Deserialize<dafs::BlockEdit>(_edit);

        dafs::BlockInfo info = edit.info;
        dafs::Delta delta = dafs::Deserialize<dafs::Delta>(edit.item);

        // 1: check hash
        // 2: update hash
        // 3: info.revision += 1
        // 4: write out info
    }
}
