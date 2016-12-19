#pragma once

#include <unordered_map>
#include <vector>

#include <paxos/parliament.hpp>

#include "callback.hpp"
#include "filesystem.hpp"
#include "propose.hpp"
#include "signal.hpp"


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
    };


    void WriteBlock(
        dafs::ProposalContent& content);
}

