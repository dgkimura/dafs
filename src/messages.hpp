#ifndef __DAFS_MESSAGES_HPP_INCLUDED__
#define __DAFS_MESSAGES_HPP_INCLUDED__

#include <string>
#include <vector>


namespace dafs
{
    struct MetaData
    {
        std::string key;
        std::string value;
    };


    struct Message
    {
        std::string from;
        std::string to;
        std::string content;
        std::vector<MetaData> metadata;
    };


    class MetaDataParser
    {
    public:

        MetaDataParser(std::vector<dafs::MetaData>);

        std::string GetValue(std::string key);

    private:

        std::vector<dafs::MetaData> metadata;
    };
}


#endif
