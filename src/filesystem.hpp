#ifndef __FILESYSTEM_HPP_INCLUDED__
#define __FILESYSTEM_HPP_INCLUDED__

#include <string>
#include <vector>


namespace dafs
{
    const int BLOCK_SIZE_IN_BYTES = 1048576;


    struct BlockInfo
    {
        std::string filename;

        std::string address;
    };


    struct BlockFormat
    {
        char contents[BLOCK_SIZE_IN_BYTES];
    };


    struct FileInfo
    {
        std::string previous;

        std::string next;

        std::string current;

        int identifier;
    };


    struct FileFormat
    {
        FileInfo info;

        std::vector<BlockInfo> blocks;
    };


    class FileDescriptor
    {
    public:

        FileDescriptor();

        void Read();

        void Write(std::string string);

        void Seek(int offset);
    };
}


#endif
