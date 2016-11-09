#pragma once

#include <fstream>

#include "filesystem.hpp"


namespace std
{
    template<> struct hash<dafs::BlockFormat>
    {
        std::size_t operator()(dafs::BlockFormat const& s) const
        {
            std::size_t h = 0;
            for (int i=0; i<dafs::BLOCK_SIZE_IN_BYTES; i++)
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
            std::fstream f(b.filename,
                            std::ios::out | std::ios::in | std::ios::binary);
            dafs::BlockFormat rawblock;
            f.read(rawblock.contents, dafs::BLOCK_SIZE_IN_BYTES);

            return std::hash<dafs::BlockFormat>{}(rawblock);
        }
    };
}
