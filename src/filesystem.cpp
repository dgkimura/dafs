#include <cstring>

#include "dafs/filesystem.hpp"


namespace dafs
{
    Bytes
    CreateBytes(const std::string& data)
    {
        Bytes bytes;
        std::memmove(bytes.contents, data.c_str(), data.length());
        return bytes;
    }


    bool
    IsLogicallyOrdered(
        dafs::Identity first,
        dafs::Identity second,
        dafs::Identity third)
    {
        return ((first <= second) && (second <= third)) ||
               ((third <= first) && (first <= second)) ||
               ((second <= third) && (third <= first));
    }
}

