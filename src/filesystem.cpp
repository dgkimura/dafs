#include <cstring>

#include "filesystem.hpp"


namespace dafs
{
    Bytes
    CreateBytes(const std::string& data)
    {
        Bytes bytes;
        std::memmove(bytes.contents, data.c_str(), data.length());
        return bytes;
    }


    Location
    CreateLocation(const std::string& address)
    {
        Location location;
        location.address = address;
        return location;
    }


    BlockInfo
    CreateBlockInfo(const std::string& filename, const dafs::Location location)
    {
        BlockInfo blockinfo;
        blockinfo.filename = filename;
        blockinfo.location = location;
        blockinfo.offset = 0;
        return blockinfo;
    }
}

