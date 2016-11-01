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

        std::string item;
    };


    Proposal CreateBlockEditProposal(
        ProposalType type,
        std::string item,
        BlockInfo block);


    class Proposer
    {
    public:

        Proposer(dafs::Storage& store);

        void operator()(std::string proposal);

    private:

        dafs::Storage& store;

        std::unordered_map<
            dafs::ProposalType,
            std::function<void(std::string proposal, dafs::Storage& store)>,
            dafs::ProposalTypeHash
        > proposal_map;
    };


    void ProposeCreateFile(std::string edit, dafs::Storage& store);


    void ProposeRemoveFile(std::string edit, dafs::Storage& store);


    void ProposeCreateBlock(std::string edit, dafs::Storage& store);


    void ProposeRemoveBlock(std::string edit, dafs::Storage& store);


    void ProposeWriteDelta(std::string edit, dafs::Storage& store);
}
