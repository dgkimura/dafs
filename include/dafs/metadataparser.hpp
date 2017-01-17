#pragma once

#include <map>
#include <string>
#include <typeindex>
#include <vector>

#include "dafs/serialization.hpp"


namespace dafs
{
    class MetaDataParser
    {
    public:

        MetaDataParser(std::vector<dafs::MetaData>);

        template<typename T>
        T GetValue(std::string key)
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
            return dafs::Deserialize<T>(value);
        }

    private:

        std::vector<dafs::MetaData> metadata;
    };

}
