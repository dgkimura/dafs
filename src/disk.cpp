#include <fstream>

#include <boost/filesystem.hpp>

#include "disk.hpp"


namespace dafs
{
    dafs::BlockFormat
    ReadBlock(dafs::BlockInfo info)
    {
        dafs::BlockFormat b;
        if (boost::filesystem::exists(info.path))
        {
            std::fstream f(fs::path(info.path).string(),
                           std::ios::in | std::ios::binary);
            std::stringstream buffer;
            buffer << f.rdbuf();
            b.contents = buffer.str();
            f.close();
        }
        return b;
    }


    void
    Write(dafs::BlockInfo info, dafs::Delta delta)
    {
        std::fstream input(fs::path(info.path).string(),
                           std::ios::in | std::ios::binary);
        std::string apply_delta_content = dafs::ApplyDelta(delta, input);

        std::fstream output(fs::path(info.path).string(),
                            std::ios::out | std::ios::binary);
        output << apply_delta_content;
    }


    dafs::Delta
    Set(dafs::BlockInfo info, std::string content,
        std::function<dafs::BlockFormat(dafs::BlockInfo)> get_block)
    {
        dafs::BlockFormat block = get_block(info);

        if (!block.contents.empty())
        {
            dafs::Delta delta = dafs::CreateDelta(
                info.path,
                block.contents,
                content);
            return delta;
        }
        else
        {
            dafs::Delta delta = dafs::CreateDelta(
                info.path,
                "",
                content);
            return delta;
        }
    }

}
