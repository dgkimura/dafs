#pragma once

#include <unordered_map>
#include <vector>

#include <boost/uuid/uuid.hpp>

#include "dafs/callback.hpp"
#include "dafs/customhash.hpp"
#include "dafs/filesystem.hpp"
#include "dafs/propose.hpp"
#include "dafs/signal.hpp"


namespace dafs
{
    class Ignore
    {
    public:

        Ignore(
            std::unordered_map<
                boost::uuids::uuid,
                std::shared_ptr<dafs::Signal>>& progress_map);

        void operator()(std::string proposal);

    private:

        std::unordered_map<
            boost::uuids::uuid,
            std::shared_ptr<dafs::Signal>
        >& progress_map;
    };


    class Commit
    {
    public:

        Commit(
            dafs::Root root,
            std::unordered_map<
                boost::uuids::uuid,
                std::shared_ptr<dafs::Signal>>& progress_map);

        void operator()(std::string proposal);

        struct Result
        {
            std::string contents;
        };

    private:

        std::unordered_map<
            boost::uuids::uuid,
            std::shared_ptr<dafs::Signal>
        >& progress_map;

        std::unordered_map<
            dafs::ProposalType,
            dafs::Callback<dafs::Commit::Result, dafs::ProposalContent&>,
            dafs::ProposalTypeHash
        > proposal_map;

        dafs::Root root;
    };


    dafs::Commit::Result WriteBlock(
        dafs::ProposalContent& content);


    dafs::Commit::Result DeleteBlock(
        dafs::ProposalContent& content);
}

