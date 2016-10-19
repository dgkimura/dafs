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


    class MetaDataParser
    {
    public:

        MetaDataParser(std::vector<dafs::MetaData>);

        std::string GetValue(std::string key);

    private:

        std::vector<dafs::MetaData> metadata;
    };
}
