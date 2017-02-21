#pragma once

#include <unordered_map>
#include <vector>

#include <paxos/parliament.hpp>

#include "dafs/callback.hpp"
#include "dafs/filesystem.hpp"
#include "dafs/propose.hpp"
#include "dafs/signal.hpp"


namespace dafs
{
    class Commit
    {
    public:

        Commit(Parliament& parliament, dafs::Signal& condition);

        void operator()(std::string proposal);

        struct Result
        {
            std::string contents;
        };

    private:

        std::unordered_map<
            dafs::ProposalType,
            dafs::Callback<dafs::Commit::Result, dafs::ProposalContent&>,
            dafs::ProposalTypeHash
        > proposal_map;

        dafs::Signal& condition;
    };


    dafs::Commit::Result WriteBlock(
        dafs::ProposalContent& content);


    dafs::Commit::Result DeleteBlock(
        dafs::ProposalContent& content);
}

