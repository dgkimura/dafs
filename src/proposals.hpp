#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include <paxos/parliament.hpp>

#include "callback.hpp"
#include "filesystem.hpp"
#include "proposaltype.hpp"


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


    class Action
    {
    public:

        Action(Parliament& parliament);

        void operator()(std::string proposal);

    private:

        std::unordered_map<
            dafs::ProposalType,
            dafs::Callback,
            dafs::ProposalTypeHash
        > proposal_map;
    };


    void ProposeCreateFile(
        std::string edit);


    void ProposeRemoveFile(
        std::string edit);


    void ProposeCreateBlock(
        std::string edit);


    void ProposeRemoveBlock(
        std::string edit);


    void ProposeWriteDelta(
        std::string edit);


    void ProposeAddNode(
        std::string edit,
        Parliament& parliament);


    void ProposeRemoveNode(
        std::string edit,
        Parliament& parliament);
}
