#pragma once

#include <functional>
#include <unordered_map>

#include "filesystem.hpp"


namespace dafs
{
    enum class ProposalType
    {
        SuperBlockInsert,
        SuperBlockRemove,
        BlockWriteDelta,
    };


    struct Proposal
    {
        //
        // Type of proposal used to deserializate the content.
        //
        ProposalType type;

        //
        // Serialized content.
        //
        std::string content;
    };


    //
    // Structure to handle adding or removing an item in block.
    //
    struct BlockEdit
    {
        dafs::BlockInfo info;

        std::string item;
    };


    Proposal CreateBlockEditProposal(
        ProposalType type,
        std::string item,
        BlockInfo block);


    void HandleProposals(std::string proposal);


    void ProposeCreateFile(std::string edit);


    void ProposeRemoveFile(std::string edit);


    void ProposeCreateBlock(std::string edit);


    void ProposeRemoveBlock(std::string edit);


    void ProposeWriteDelta(std::string edit);
}
