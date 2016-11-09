#pragma once

#include <functional>
#include <unordered_map>

#include "filesystem.hpp"
#include "proposaltype.hpp"
#include "storage.hpp"


namespace dafs
{
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

        std::string change;

        std::size_t hash;

        int revision;
    };


    Proposal CreateBlockEditProposal(
        ProposalType type,
        std::string item,
        BlockInfo block,
        int revision);


    void ProposeCreateFile(std::string edit);


    void ProposeRemoveFile(std::string edit);


    void ProposeCreateBlock(std::string edit);


    void ProposeRemoveBlock(std::string edit);


    void ProposeWriteDelta(std::string edit);


    const std::unordered_map<
        dafs::ProposalType,
        std::function<void(std::string proposal)>,
        dafs::ProposalTypeHash
        > proposal_map =
    {
        {ProposalType::CreateFile, ProposeCreateFile},
        {ProposalType::RemoveFile, ProposeRemoveFile},
        {ProposalType::CreateBlock, ProposeCreateBlock},
        {ProposalType::RemoveBlock, ProposeRemoveBlock},
        {ProposalType::WriteDelta, ProposeWriteDelta}
    };
}
