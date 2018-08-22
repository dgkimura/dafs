#include <fstream>
#include <sstream>


#include <boost/filesystem.hpp>

#include "dafs/blocks.hpp"
#include "dafs/disk.hpp"


namespace dafs
{
    dafs::BlockFormat
    DeleteBlock(dafs::BlockInfo info)
    {
        dafs::BlockFormat b = dafs::ReadBlock(info);
        if (boost::filesystem::exists(info.path))
        {
            std::remove(boost::filesystem::path(info.path).string().c_str());
        }
        return b;
    }


    dafs::BlockFormat
    ReadBlock(dafs::BlockInfo info)
    {
        dafs::BlockFormat b;
        if (boost::filesystem::exists(info.path))
        {
            std::fstream f(boost::filesystem::path(info.path).string(),
                           std::ios::in | std::ios::binary);
            std::stringstream buffer;
            buffer << f.rdbuf();
            b.contents = buffer.str();
            f.close();
        }
        return b;
    }


    void
    Write(std::string path, dafs::Delta delta)
    {
        std::fstream input(boost::filesystem::path(path).string(),
                           std::ios::in | std::ios::binary);
        std::string apply_delta_content = dafs::ApplyDelta(delta, input);

        std::fstream output(boost::filesystem::path(path).string(),
                            std::ios::out | std::ios::binary);
        output << apply_delta_content;
        output.flush();
    }


    dafs::Delta
    Insert(std::iostream& stream, dafs::BlockInfo item)
    {
        std::stringstream buffer;
        buffer << stream.rdbuf();
        auto original = dafs::deserialize<dafs::BlockIndex>(buffer.str());
        auto newset = original;
        newset.items.push_back(item);

        dafs::Delta delta = dafs::CreateDelta(
            dafs::serialize(original),
            dafs::serialize(newset));
        return delta;
    }


    dafs::Delta
    Remove(std::iostream& stream, dafs::BlockInfo item)
    {
        std::stringstream buffer;
        buffer << stream.rdbuf();
        auto original = dafs::deserialize<dafs::BlockIndex>(buffer.str());
        auto newset = original;
        newset.items.erase
        (
            std::remove_if
            (
                newset.items.begin(),
                newset.items.end(),
                [=](const dafs::BlockInfo& current)
                {
                    return current == item;
                }
            ),
            newset.items.end()
        );

        dafs::Delta delta = dafs::CreateDelta(
            dafs::serialize(original),
            dafs::serialize(newset));
        return delta;
    }
}
