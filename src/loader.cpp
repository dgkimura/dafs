#include <fstream>

#include <boost/filesystem.hpp>

#include "dafs/loader.hpp"
#include "dafs/serialization.hpp"


namespace dafs
{
    dafs::LocationIndex
    Loader::GetLocations(dafs::BlockInfo nodes)
    {
        dafs::LocationIndex locations;

        if (boost::filesystem::exists(nodes.path))
        {
            std::fstream f(boost::filesystem::path(nodes.path).string(),
                           std::ios::in | std::ios::binary);

            locations = dafs::DeserializeFromBlockFormat<LocationIndex>(f);
        }
        return locations;
    }
}
