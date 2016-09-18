#ifndef __FILESYSTEM_HPP_INCLUDED__
#define __FILESYSTEM_HPP_INCLUDED__

#include <string>
#include <vector>


namespace dafs
{
    const int BLOCK_SIZE_IN_BYTES = 1048576;

    struct Block
    {
        Block(std::string contents_)
        {
            contents_.copy(contents, contents_.length());
        }

        char contents[BLOCK_SIZE_IN_BYTES];
    };


    struct BlockInfo
    {
        std::string filename;
    };


    class MetaFile
    {
    };


    class File
    {
    public:

        File();

        void Read();

        void Write(std::string string);

        void Seek(int offset);

    private:

        unsigned int read_pointer;

        unsigned int write_pointer;

        MetaFile previous;

        MetaFile current;

        MetaFile next;
    };


    class SuperBlock
    {
    private:

        std::vector<File> files;

        std::vector<Block> blocks;
    };
}


#endif
