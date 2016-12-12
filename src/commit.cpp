#include <vector>

#include "boost/algorithm/string.hpp"
#include "boost/lexical_cast.hpp"

#include "commit.hpp"
#include "customhash.hpp"
#include "disk.hpp"
#include "propose.hpp"
#include "serialization.hpp"


namespace dafs
{
    Commit::Commit(Parliament& parliament, dafs::Signal& condition)
        : proposal_map
          {
              {
                   ProposalType::WriteBlock,
                   dafs::Callback<dafs::ProposalContent&>
                   (
                       [&condition](dafs::ProposalContent& context)
                       {
                           dafs::WriteBlock(context);
                           condition.Set();
                       }
                   )
              },
              {
                   ProposalType::AddNode,
                   dafs::Callback<dafs::ProposalContent&>
                   (
                       [&parliament, &condition](dafs::ProposalContent& context)
                       {
                           dafs::WriteBlock(context);
                           dafs::AddNode(context, parliament);
                           condition.Set();
                       }
                   )
              },
              {
                   ProposalType::RemoveNode,
                   dafs::Callback<dafs::ProposalContent&>
                   (
                       [&parliament, &condition](dafs::ProposalContent& context)
                       {
                           dafs::WriteBlock(context);
                           dafs::RemoveNode(context, parliament);
                           condition.Set();
                       }
                   )
              }
          }
    {
    }


    void
    Commit::operator()(std::string proposal)
    {
        Proposal p = dafs::Deserialize<dafs::Proposal>(proposal);
        dafs::ProposalContent edit = dafs::Deserialize<dafs::ProposalContent>(p.content);

        proposal_map[p.type](edit);
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


    void
    AddNode(
        dafs::ProposalContent& edit,
        Parliament& parliament)
    {
        //
        // Check hash and revision of block info list.
        //
        // TODO: Add revision check
        if (edit.hash == std::hash<dafs::BlockInfo>{}(edit.info))
        {
            // TODO: Parse delta to find node added

            //
            // Update running node set.
            //
            std::vector<std::string> hostport;
            split(hostport, edit.change, boost::is_any_of(":"));
            parliament.AddLegislator(hostport[0],
                                     boost::lexical_cast<short>(hostport[1]));
        }
    }


    void
    RemoveNode(
        dafs::ProposalContent& edit,
        Parliament& parliament)
    {
        //
        // Check hash and revision of block info list.
        //
        // TODO: Add revision check
        if (edit.hash == std::hash<dafs::BlockInfo>{}(edit.info))
        {
            // TODO: Parse delta to find node removed

            //
            // Update running node set.
            //
            std::vector<std::string> hostport;
            split(hostport, edit.change, boost::is_any_of(":"));
            parliament.RemoveLegislator(hostport[0],
                                        boost::lexical_cast<short>(hostport[1]));
        }
    }
}
