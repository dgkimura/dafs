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
        {ProposalType::SuperBlockInsert, HandleSuperBlockInsert},
        {ProposalType::SuperBlockRemove, HandleSuperBlockRemove},
        {ProposalType::BlockWriteDelta, HandleWriteDelta}
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
    HandleSuperBlockInsert(std::string _edit)
    {
        dafs::BlockEdit edit = dafs::Deserialize<dafs::BlockEdit>(_edit);

        dafs::BlockInfo info = edit.info;
        dafs::FileInfo file = dafs::Deserialize<dafs::FileInfo>(edit.item);

        // TODO: check hash
    }


    void
    HandleSuperBlockRemove(std::string _edit)
    {
        dafs::BlockEdit edit = dafs::Deserialize<dafs::BlockEdit>(_edit);

        dafs::BlockInfo info = edit.info;
        dafs::FileInfo file = dafs::Deserialize<dafs::FileInfo>(edit.item);

        // TODO: check hash
    }


    void
    HandleWriteDelta(std::string _edit)
    {
        dafs::BlockEdit edit = dafs::Deserialize<dafs::BlockEdit>(_edit);

        dafs::BlockInfo info = edit.info;
        dafs::Delta delta = dafs::Deserialize<dafs::Delta>(edit.item);

        // TODO: check hash
    }
}
