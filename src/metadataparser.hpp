#pragma once

#include <string>
#include <vector>

#include "serialization.hpp"


namespace dafs
{
    class MetaDataParser
    {
    public:

        MetaDataParser(std::vector<dafs::MetaData>);

        template<typename T>
        std::shared_ptr<T> GetValue(std::string key)
        {
            std::string value;
            for (auto data : metadata)
            {
                if (data.key == key)
                {
                    value= data.value;
                    break;
                }
            }
            return value.empty() ? nullptr : std::make_shared<T>(dafs::Deserialize<T>(value));
        }

    private:

        std::vector<dafs::MetaData> metadata;
    };

}
