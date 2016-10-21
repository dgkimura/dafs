#pragma once

#include <string>
#include <vector>


namespace dafs
{
    struct MetaData
    {
        std::string key;
        std::string value;
    };


    const std::string FileInfoKey = "FileInfo";


    const std::string BlockInfoKey = "BlockInfo";


    const std::string BytesKey = "Bytes";
}
