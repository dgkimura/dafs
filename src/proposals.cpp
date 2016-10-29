#include "proposals.hpp"
#include "serialization.hpp"


namespace dafs
{
    struct EnumHasher
    {
        template<typename T>
        std::size_t operator()(T t) const
        {
            return static_cast<std::size_t>(t);
        }
    };


    const std::unordered_map<
        ProposalType, std::function<void(std::string proposal)>,
        EnumHasher
    > ProposalMapHandler =
    {
        {ProposalType::SuperBlockInsert, ProposeCreateFile},
        {ProposalType::SuperBlockRemove, ProposeRemoveFile},
        {ProposalType::BlockWriteDelta, ProposeWriteDelta}
    };


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


    void
    HandleProposals(std::string proposal)
    {
        Proposal p = dafs::Deserialize<dafs::Proposal>(proposal);
        ProposalMapHandler.at(p.type)(p.content);
    }


    void
    ProposeCreateFile(std::string _edit)
    {
        dafs::BlockEdit edit = dafs::Deserialize<dafs::BlockEdit>(_edit);

        dafs::BlockInfo info = edit.info;
        dafs::FileInfo file = dafs::Deserialize<dafs::FileInfo>(edit.item);

        // 1: check hash and revision of superblock
        // 2: add file to superblock with revision 0
    }


    void
    ProposeRemoveFile(std::string _edit)
    {
        dafs::BlockEdit edit = dafs::Deserialize<dafs::BlockEdit>(_edit);

        dafs::BlockInfo info = edit.info;
        dafs::FileInfo file = dafs::Deserialize<dafs::FileInfo>(edit.item);

        // 1: check hash and revision of superblock
        // 2: delete file from superblock
    }


    void
    ProposeCreateBlock(std::string _edit)
    {
    }


    void
    ProposeRemoveBlock(std::string _edit)
    {
    }


    void
    ProposeWriteDelta(std::string _edit)
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
