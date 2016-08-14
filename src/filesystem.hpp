#ifndef __FILESYSTEM_HPP_INCLUDED__
#define __FILESYSTEM_HPP_INCLUDED__

#include <string>


class Metadata
{
};


class Block
{
};


class File
{
public:

    File();

    void Read();

    void Write(std::string string);

private:

    unsigned int read_pointer;

    unsigned int write_pointer;
};


#endif
