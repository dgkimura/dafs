#pragma once

#include <map>
#include <string>
#include <typeindex>
#include <vector>

#include "serialization.hpp"


namespace dafs
{
    class MetaDataParser
    {
    public:

        MetaDataParser(std::vector<dafs::MetaData>);

        template<typename T>
        T GetValue()
        {
            std::string value;
            for (auto data : metadata)
            {
                if (data.key == map_type[typeid(T)])
                {
                    value= data.value;
                    break;
                }
            }
            return dafs::Deserialize<T>(value);
        }

    private:

        std::vector<dafs::MetaData> metadata;

        std::map<std::type_index, std::string> map_type;
    };

}
