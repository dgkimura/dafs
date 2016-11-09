#include "customhash.hpp"
#include "proposals.hpp"
#include "serialization.hpp"


namespace dafs
{
    Proposal
    CreateBlockEditProposal(
        ProposalType type,
        std::string item,
        BlockInfo info,
        int revision)
    {
        BlockEdit edit;
        edit.info = info;
        edit.change = item;
        edit.hash = std::hash<dafs::BlockInfo>{}(info);

        Proposal p;
        p.type = type;
        p.content = dafs::Serialize(edit);
        return p;
    }


    void
    GlobalProposalHandler(std::string proposal)
    {
        Proposal p = dafs::Deserialize<dafs::Proposal>(proposal);
        proposal_map.at(p.type)(p.content);
    }


    void
    ProposeCreateFile(std::string _edit)
    {
        dafs::BlockEdit edit = dafs::Deserialize<dafs::BlockEdit>(_edit);

        dafs::FileInfo file = dafs::Deserialize<dafs::FileInfo>(edit.change);

        std::fstream s(edit.info.filename,
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
    ProposeRemoveFile(std::string _edit)
    {
        dafs::BlockEdit edit = dafs::Deserialize<dafs::BlockEdit>(_edit);

        dafs::FileInfo file = dafs::Deserialize<dafs::FileInfo>(edit.change);

        std::fstream s(edit.info.filename,
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
                        return file.name == f.name;
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
    ProposeCreateBlock(std::string _edit)
    {
        dafs::BlockEdit edit = dafs::Deserialize<dafs::BlockEdit>(_edit);

        std::fstream s(edit.info.filename,
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
    ProposeRemoveBlock(std::string _edit)
    {
        dafs::BlockEdit edit = dafs::Deserialize<dafs::BlockEdit>(_edit);

        std::fstream s(edit.info.filename,
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
                        return block.filename == b.filename;
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
    ProposeWriteDelta(std::string _edit)
    {
        dafs::BlockEdit edit = dafs::Deserialize<dafs::BlockEdit>(_edit);

        std::fstream s(edit.info.filename,
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
}
