#ifndef __FILESYSTEM_HPP_INCLUDED__
#define __FILESYSTEM_HPP_INCLUDED__

#include <string>
#include <vector>


namespace dafs
{
    const int BLOCK_SIZE_IN_BYTES = 65536;

    const int FILEINFO_SIZE_IN_BLOCKS = 8192;


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

        int identifier;
    };


    struct FileFormat
    {
        FileInfo info;

        BlockInfo blocks[FILEINFO_SIZE_IN_BLOCKS];
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
