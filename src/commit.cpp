#include <iostream>
#include <unordered_map>
#include "boost/algorithm/string.hpp"
#include "boost/lexical_cast.hpp"

#include "dafs/commit.hpp"
#include "dafs/customhash.hpp"
#include "dafs/disk.hpp"
#include "dafs/propose.hpp"
#include "dafs/serialization.hpp"


namespace dafs
{
    Ignore::Ignore(
        std::unordered_map<boost::uuids::uuid, std::shared_ptr<dafs::Signal>>& progress_map)
        : progress_map(progress_map)
    {
    }


    void
    Ignore::operator()(std::string proposal)
    {
        Proposal p = dafs::Deserialize<dafs::Proposal>(proposal);

        if (progress_map.find(p.uuid) != progress_map.end())
        {
            dafs::Result r;
            r.complete = true;
            r.success = false;
            progress_map[p.uuid]->Set(r);
        }
    }


    Commit::Commit(
        dafs::Root root,
        std::unordered_map<boost::uuids::uuid, std::shared_ptr<dafs::Signal>>& progress_map)
        : progress_map(progress_map),
          proposal_map
          {
              {
                   ProposalType::WriteBlock,
                   dafs::Callback<dafs::Result, dafs::ProposalContent&>
                   (
                       [](dafs::ProposalContent& context) -> dafs::Result
                       {
                           return dafs::WriteBlock(context);
                       }
                   )
                },
                {
                   ProposalType::DeleteBlock,
                   dafs::Callback<dafs::Result, dafs::ProposalContent&>
                   (
                       [](dafs::ProposalContent& context) -> dafs::Result
                       {
                           return dafs::DeleteBlock(context);
                       }
                   )
                },
                {
                   ProposalType::Ping,
                   dafs::Callback<dafs::Result, dafs::ProposalContent&>
                   (
                       [](dafs::ProposalContent& context) -> dafs::Result
                       {
                            dafs::Result r;
                            return r;
                       }
                   )
              }
          },
          root(root)
    {
    }


    void
    Commit::operator()(std::string proposal)
    {
        Proposal p = dafs::Deserialize<dafs::Proposal>(proposal);
        dafs::ProposalContent edit = dafs::Deserialize<dafs::ProposalContent>(p.content);

        edit.info.path = (boost::filesystem::path(root.directory) /
                         boost::filesystem::path(edit.info.path)).string();
        auto result = proposal_map[p.type](edit);
        result.complete = true;

        if (progress_map.find(p.uuid) != progress_map.end())
        {
            progress_map[p.uuid]->Set(result);
        }
    }


    dafs::Result
    WriteBlock(
        dafs::ProposalContent& edit)
    {
        dafs::Result r;

        //
        // Check hash and revision of block info list.
        //
        // TODO: Add revision check
        if (edit.hash == std::hash<dafs::BlockInfo>{}(edit.info))
        {
            dafs::Delta delta = dafs::Deserialize<dafs::Delta>(edit.change);
            dafs::Write(edit.info.path, delta);

            r.success = true;
        }
        else
        {
            r.success = false;
        }
        return r;
    }


    dafs::Result
    DeleteBlock(
        dafs::ProposalContent& edit)
    {
        dafs::Result r;

        //
        // Check hash and revision of block info list.
        //
        // TODO: Add revision check
        if (edit.hash == std::hash<dafs::BlockInfo>{}(edit.info))
        {
            r.content = dafs::Serialize(dafs::DeleteBlock(edit.info));
            r.success = true;
        }
        else
        {
            r.success = false;
        }
        return r;
    }
}
