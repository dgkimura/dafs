#include <gtest/gtest.h>

#include "dafs/commit.hpp"


TEST(CommitTest, testIgnoredCommitHasUnsuccessfulResult)
{
    auto uuid = boost::uuids::nil_uuid();
    auto signal = std::make_shared<dafs::Signal>([](){});

    std::unordered_map<boost::uuids::uuid, std::shared_ptr<dafs::Signal>> progress_map;
    progress_map[uuid] = signal;

    dafs::Ignore ignore(progress_map);
    ignore(
        dafs::Serialize(
            dafs::Proposal
            {
                dafs::ProposalType::Ping,
                "",
                uuid
            }
    ));

    ASSERT_FALSE(progress_map[uuid]->Wait().success);
}
