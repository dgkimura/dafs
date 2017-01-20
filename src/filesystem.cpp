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


    BlockInfo
    CreateBlockInfo(const std::string& path, const dafs::Identity identity)
    {
        BlockInfo blockinfo;
        blockinfo.path = path;
        blockinfo.identity = identity;
        return blockinfo;
    }
}

