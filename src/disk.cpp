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
}
