#include <vector>

#include "boost/algorithm/string.hpp"
#include "boost/lexical_cast.hpp"

#include "customhash.hpp"
#include "proposals.hpp"
#include "serialization.hpp"


namespace dafs
{
    Proposal
    CreateProposal(
        ProposalType type,
        std::string item,
        BlockInfo info,
        int revision)
    {
        ProposalContent edit;
        edit.info = info;
        edit.change = item;
        edit.hash = std::hash<dafs::BlockInfo>{}(info);

        Proposal p;
        p.type = type;
        p.content = dafs::Serialize(edit);
        return p;
    }


    Action::Action(Parliament& parliament)
        : proposal_map
          {
              {
                   ProposalType::CreateFile,
                   dafs::Callback<ProposalContent&>
                   (
                       [](ProposalContent& context)
                       {
                           CreateFile(context);
                       }
                   )
              },
              {
                   ProposalType::RemoveFile,
                   dafs::Callback<ProposalContent&>
                   (
                       [](ProposalContent& context)
                       {
                           RemoveFile(context);
                       }
                   )
              },
              {
                   ProposalType::CreateBlock,
                   dafs::Callback<ProposalContent&>
                   (
                       [](ProposalContent& context)
                       {
                           CreateBlock(context);
                       }
                   )
              },
              {
                   ProposalType::RemoveBlock,
                   dafs::Callback<ProposalContent&>
                   (
                       [](ProposalContent& context)
                       {
                           RemoveBlock(context);
                       }
                   )
              },
              {
                   ProposalType::WriteDelta,
                   dafs::Callback<ProposalContent&>
                   (
                       [](ProposalContent& context)
                       {
                           WriteDelta(context);
                       }
                   )
              },
              {
                   ProposalType::AddNode,
                   dafs::Callback<ProposalContent&>
                   (
                       [&parliament](ProposalContent& context)
                       {
                           WriteDelta(context);
                           AddNode(context, parliament);
                       }
                   )
              },
              {
                   ProposalType::RemoveNode,
                   dafs::Callback<ProposalContent&>
                   (
                       [&parliament](ProposalContent& context)
                       {
                           WriteDelta(context);
                           RemoveNode(context, parliament);
                       }
                   )
              }
          }
    {
    }


    void
    Action::operator()(std::string proposal)
    {
        Proposal p = dafs::Deserialize<dafs::Proposal>(proposal);
        dafs::ProposalContent edit = dafs::Deserialize<dafs::ProposalContent>(p.content);

        proposal_map[p.type](edit);
    }


    void
    CreateFile(
        dafs::ProposalContent& edit)
    {
        dafs::FileInfo file = dafs::Deserialize<dafs::FileInfo>(edit.change);

        std::fstream s(edit.info.path,
                       std::ios::out | std::ios::in | std::ios::binary);
        dafs::FileIndex index = dafs::Deserialize<dafs::FileIndex>(s);

        //
        // Check hash and revision of block info list.
        //
        // TODO: Add revision check
        if (edit.hash == std::hash<dafs::BlockInfo>{}(edit.info))
        {
            //
            // Add file to file info list.
            //
            index.files.push_back(file);

            //
            // Write out updated file info list.
            //
            s << dafs::Serialize<dafs::FileIndex>(index);
            s.flush();
        }
    }


    void
    RemoveFile(
        dafs::ProposalContent& edit)
    {
        dafs::FileInfo file = dafs::Deserialize<dafs::FileInfo>(edit.change);

        std::fstream s(edit.info.path,
                       std::ios::out | std::ios::in | std::ios::binary);
        dafs::BlockFormat block = dafs::Deserialize<dafs::BlockFormat>(s);

        //
        // Check hash and revision of block info list.
        //
        // TODO: Add revision check
        if (edit.hash == std::hash<dafs::BlockInfo>{}(edit.info))
        {
            //
            // Delete file info from file info list.
            //
            dafs::FileIndex index = dafs::Deserialize<dafs::FileIndex>(
                                        block.contents);
            index.files.erase
            (
                std::remove_if
                (
                    index.files.begin(),
                    index.files.end(),
                    [=](const dafs::FileInfo& f)
                    {
                        return file.path == f.path;
                    }
                ),
                index.files.end()
            );

            //
            // Write out updated file info list.
            //
            s << dafs::Serialize<dafs::FileIndex>(index);
            s.flush();
        }
    }


    void
    CreateBlock(
        dafs::ProposalContent& edit)
    {
        std::fstream s(edit.info.path,
                       std::ios::out | std::ios::in | std::ios::binary);
        dafs::BlockIndex index = dafs::Deserialize<dafs::BlockIndex>(s);

        //
        // Check hash and revision of block info list.
        //
        // TODO: Add revision check
        if (edit.hash == std::hash<dafs::BlockInfo>{}(edit.info))
        {
            //
            // Add block to block info list.
            //
            dafs::BlockInfo block = dafs::Deserialize<dafs::BlockInfo>(edit.change);
            index.blocks.push_back(block);

            //
            // Write out updated block info list.
            //
            s << dafs::Serialize<dafs::BlockIndex>(index);
            s.flush();
        }
    }


    void
    RemoveBlock(
        dafs::ProposalContent& edit)
    {
        std::fstream s(edit.info.path,
                       std::ios::out | std::ios::in | std::ios::binary);
        dafs::BlockIndex index = dafs::Deserialize<dafs::BlockIndex>(s);

        dafs::BlockInfo block = dafs::Deserialize<dafs::BlockInfo>(edit.change);

        //
        // Check hash and revision of block info list.
        //
        // TODO: Add revision check
        if (edit.hash == std::hash<dafs::BlockInfo>{}(edit.info))
        {
            //
            // Delete block info from block info list.
            //
            index.blocks.erase
            (
                std::remove_if
                (
                    index.blocks.begin(),
                    index.blocks.end(),
                    [=](const dafs::BlockInfo& b)
                    {
                        return block.path == b.path;
                    }
                ),
                index.blocks.end()
            );

            //
            // Write out updated file info list.
            //
            s << dafs::Serialize<dafs::BlockIndex>(index);
            s.flush();
        }
    }


    void
    WriteDelta(
        dafs::ProposalContent& edit)
    {
        std::fstream s(edit.info.path,
                       std::ios::out | std::ios::in | std::ios::binary);
        dafs::Delta delta = dafs::Deserialize<dafs::Delta>(edit.change);

        //
        // Check hash and revision of block info list.
        //
        // TODO: Add revision check
        if (edit.hash == std::hash<dafs::BlockInfo>{}(edit.info))
        {
            // write out info
            dafs::BlockFormat block = dafs::Deserialize<dafs::BlockFormat>(s);
            s << dafs::ApplyDelta(delta, block.contents);
            s.flush();
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
