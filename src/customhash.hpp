#pragma once

#include <fstream>
#include <sys/stat.h>

#include "filesystem.hpp"
#include "serialization.hpp"


namespace std
{
    template<> struct hash<dafs::BlockFormat>
    {
        std::size_t operator()(dafs::BlockFormat const& b) const
        {
            std::size_t h = 0;
            for (int i=0; i<b.contents.length(); i++)
            {
                h ^= std::hash<char>{}(b.contents[i]);
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
            dafs::BlockFormat rawblock = dafs::Deserialize<dafs::BlockFormat>(f);
            return std::hash<dafs::BlockFormat>{}(rawblock);
        }
    };
}
