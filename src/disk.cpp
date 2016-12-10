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
            b = dafs::Deserialize<dafs::BlockFormat>(f);
            f.close();
        }
        return b;
    }


    void
    WriteBlock(dafs::BlockInfo info, dafs::Delta delta)
    {
        std::fstream s(info.path,
                       std::ios::in | std::ios::out | std::ios::binary);
        s.seekg(0, std::ios::beg);
        s << dafs::ApplyDelta(delta, s);
        s.close();
    }
}
