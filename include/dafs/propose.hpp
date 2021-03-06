#pragma once

#include <unordered_map>
#include <vector>

#include <boost/uuid/uuid.hpp>

#include "dafs/blocks.hpp"
#include "dafs/callback.hpp"
#include "dafs/signal.hpp"


namespace dafs
{
    enum class ProposalType
    {
        WriteBlock,
        DeleteBlock,

        Ping
    };


    //
    // Hash function for proposal types
    //
    struct ProposalTypeHash
    {
        template<typename T>
        std::size_t operator()(T t) const
        {
            return static_cast<std::size_t>(t);
        }
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

        //
        // Unique identifier of proposal.
        //
        boost::uuids::uuid uuid;
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
        int revision,
        size_t hash);
}
