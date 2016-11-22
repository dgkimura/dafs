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
    struct ProposalContent
    {
        dafs::BlockInfo info;

        std::string change;

        std::size_t hash;

        int revision;
    };


    Proposal CreateProposal(
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
            dafs::Callback<ProposalContent&>,
            dafs::ProposalTypeHash
        > proposal_map;
    };


    void CreateFile(
        ProposalContent& content);


    void RemoveFile(
        ProposalContent& content);


    void CreateBlock(
        ProposalContent& content);


    void RemoveBlock(
        ProposalContent& content);


    void WriteDelta(
        ProposalContent& content);


    void AddNode(
        ProposalContent& content,
        Parliament& parliament);


    void RemoveNode(
        ProposalContent& content,
        Parliament& parliament);
}
