#pragma once

#include <fstream>
#include <sys/stat.h>

#include "filesystem.hpp"


namespace std
{
    template<> struct hash<dafs::BlockFormat>
    {
        std::size_t operator()(dafs::BlockFormat const& s, const int size=dafs::BLOCK_SIZE_IN_BYTES) const
        {
            std::size_t h = 0;
            for (int i=0; i<size; i++)
            {
                h ^= std::hash<char>{}(s.contents[i]);
            }
            return h;
        }
    };


    template<> struct hash<dafs::BlockInfo>
    {
        std::size_t operator()(dafs::BlockInfo const& b) const
        {
            std::fstream f(b.path,
                           std::ios::in | std::ios::binary);
            dafs::BlockFormat rawblock;

            size_t size = 0;
            struct stat st;
            if (stat(b.path.c_str(), &st) == 0)
            {
                size = st.st_size;
            }
            f.read(rawblock.contents, size);

            return std::hash<dafs::BlockFormat>{}(rawblock, size);
        }
    };
}
