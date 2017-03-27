#include "boost/algorithm/string.hpp"
#include "boost/lexical_cast.hpp"

#include "dafs/commit.hpp"
#include "dafs/customhash.hpp"
#include "dafs/disk.hpp"
#include "dafs/propose.hpp"
#include "dafs/serialization.hpp"


namespace dafs
{
    Commit::Commit(Parliament& parliament, dafs::Root root, dafs::Signal& condition)
        : proposal_map
          {
              {
                   ProposalType::WriteBlock,
                   dafs::Callback<dafs::Commit::Result, dafs::ProposalContent&>
                   (
                       [](dafs::ProposalContent& context) -> dafs::Commit::Result
                       {
                           return dafs::WriteBlock(context);
                       }
                   )
                },
                {
                   ProposalType::DeleteBlock,
                   dafs::Callback<dafs::Commit::Result, dafs::ProposalContent&>
                   (
                       [](dafs::ProposalContent& context) -> dafs::Commit::Result
                       {
                           return dafs::DeleteBlock(context);
                       }
                   )
                },
                {
                   ProposalType::Ping,
                   dafs::Callback<dafs::Commit::Result, dafs::ProposalContent&>
                   (
                       [](dafs::ProposalContent& context) -> dafs::Commit::Result
                       {
                            dafs::Commit::Result r;
                            return r;
                       }
                   )
              }
          },
          root(root),
          condition(condition)
    {
    }


    void
    Commit::operator()(std::string proposal)
    {
        Proposal p = dafs::Deserialize<dafs::Proposal>(proposal);
        dafs::ProposalContent edit = dafs::Deserialize<dafs::ProposalContent>(p.content);

        edit.info.path = (boost::filesystem::path(root.directory) /
                         boost::filesystem::path(edit.info.path)).string();
        proposal_map[p.type](edit);
        condition.Set();
    }


    dafs::Commit::Result
    WriteBlock(
        dafs::ProposalContent& edit)
    {
        dafs::Commit::Result r;

        //
        // Check hash and revision of block info list.
        //
        // TODO: Add revision check
        if (edit.hash == std::hash<dafs::BlockInfo>{}(edit.info))
        {
            dafs::Delta delta = dafs::Deserialize<dafs::Delta>(edit.change);
            dafs::Write(edit.info, delta);
        }
        return r;
    }


    dafs::Commit::Result
    DeleteBlock(
        dafs::ProposalContent& edit)
    {
        dafs::Commit::Result r;

        //
        // Check hash and revision of block info list.
        //
        // TODO: Add revision check
        if (edit.hash == std::hash<dafs::BlockInfo>{}(edit.info))
        {
            r.contents = dafs::Serialize(dafs::DeleteBlock(edit.info));
        }
        return r;
    }
}
