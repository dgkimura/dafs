#include <fstream>

#include <boost/filesystem.hpp>

#include "dafs/disk.hpp"


namespace dafs
{
    dafs::BlockFormat
    DeleteBlock(dafs::BlockInfo info)
    {
        dafs::BlockFormat b = dafs::ReadBlock(info);
        if (boost::filesystem::exists(info.path))
        {
            std::remove(fs::path(info.path).string().c_str());
        }
        return b;
    }


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
}
