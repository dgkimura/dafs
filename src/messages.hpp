#ifndef __DAFS_MESSAGES_HPP_INCLUDED__
#define __DAFS_MESSAGES_HPP_INCLUDED__

#include <string>
#include <vector>


namespace dafs
{
    enum class MessageType
    {
        //
        // Create a file inside the distributed system.
        //
        CreateFile,

        //
        // Delete a file from the distributed system.
        //
        DeleteFile,

        //
        // Open a file for further operations.
        //
        OpenFile,

        //
        // Read from an opened file.
        //
        ReadFile,

        //
        // Write to an opened file.
        //
        WriteFile
    };


    struct MetaData
    {
        std::string key;
        std::string value;
    };


    const std::string FileNameKey = "FileName";


    struct Message
    {
        std::string from;
        std::string to;
        std::string content;
        MessageType type;
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
