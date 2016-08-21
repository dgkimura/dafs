#ifndef __FILESYSTEM_HPP_INCLUDED__
#define __FILESYSTEM_HPP_INCLUDED__

#include <string>
#include <vector>


namespace dafs
{
    class Metadata
    {
    };


    struct Revision
    {
        int hash;
    };


    struct Owner
    {
        std::string address;
    };


    struct Contents
    {
        std::string contents;
    };


    struct Block
    {
        Revision revision;

        Owner owner;

        Contents contents;
    };


    class File
    {
    public:

        File();

        void Read();

        void Write(std::string string, int offset);

    private:

        unsigned int read_pointer;

        unsigned int write_pointer;
    };


    class SuperBlock
    {
    private:

        std::vector<File> files;

        std::vector<Block> blocks;
    };
}


#endif
