#include "messages.hpp"


namespace dafs
{
    MetaDataParser::MetaDataParser(std::vector<dafs::MetaData> metadata_)
    {
        metadata = metadata_;
    }

    std::string
    MetaDataParser::GetValue(std::string key)
    {
        std::string value;
        for (auto data : metadata)
        {
            if (data.key == key)
            {
                value = data.value;
                break;
            }
        }
        return value;
    }
}
