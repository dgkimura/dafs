#include <vector>

#include "boost/algorithm/string.hpp"
#include "boost/lexical_cast.hpp"

#include "dafs/commit.hpp"
#include "dafs/customhash.hpp"
#include "dafs/disk.hpp"
#include "dafs/propose.hpp"
#include "dafs/serialization.hpp"


namespace dafs
{
    Commit::Commit(Parliament& parliament, dafs::Signal& condition)
        : proposal_map
          {
              {
                   ProposalType::WriteBlock,
                   dafs::Callback<dafs::ProposalContent&>
                   (
                       [](dafs::ProposalContent& context)
                       {
                           dafs::WriteBlock(context);
                       }
                   )
              }
          },
          condition(condition)
    {
    }


    void
    Commit::operator()(std::string proposal)
    {
        Proposal p = dafs::Deserialize<dafs::Proposal>(proposal);
        dafs::ProposalContent edit = dafs::Deserialize<dafs::ProposalContent>(p.content);

        proposal_map[p.type](edit);
        condition.Set();
    }


    void
    WriteBlock(
        dafs::ProposalContent& edit)
    {
        //
        // Check hash and revision of block info list.
        //
        // TODO: Add revision check
        if (edit.hash == std::hash<dafs::BlockInfo>{}(edit.info))
        {
            dafs::Delta delta = dafs::Deserialize<dafs::Delta>(edit.change);
            dafs::Write(edit.info, delta);
        }
    }
}
