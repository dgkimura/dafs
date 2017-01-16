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

    private:

        std::unordered_map<
            dafs::ProposalType,
            dafs::Callback<dafs::ProposalContent&>,
            dafs::ProposalTypeHash
        > proposal_map;

        dafs::Signal& condition;
    };


    void WriteBlock(
        dafs::ProposalContent& content);
}

