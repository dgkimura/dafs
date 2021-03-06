#pragma once

#include <fstream>
#include <sys/stat.h>

#include <boost/filesystem.hpp>
#include <boost/functional/hash.hpp>
#include <boost/uuid/uuid.hpp>

#include "dafs/blocks.hpp"
#include "dafs/serialization.hpp"
#include "dafs/messages.hpp"


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
            if (!boost::filesystem::exists(b.path))
            {
                return 0;
            }
            std::fstream f(b.path,
                           std::ios::in | std::ios::binary);
            dafs::BlockFormat rawblock;
            f >> rawblock.contents;
            return std::hash<dafs::BlockFormat>{}(rawblock);
        }
    };


    template<> struct hash<dafs::Address>
    {
        std::size_t operator()(dafs::Address const& a) const
        {
            return std::hash<std::string>{}(a.ip) ^ std::hash<short>{}(a.port);
        }
    };


    template<> struct hash<boost::uuids::uuid>
    {
        std::size_t operator()(boost::uuids::uuid const& u) const
        {
            return boost::hash<boost::uuids::uuid>()(u);
        }
    };


    template<> struct equal_to<dafs::Address>
    {
        bool operator()(const dafs::Address& lhs, const dafs::Address& rhs) const
        {
            return lhs.ip == rhs.ip && lhs.port == rhs.port;
        }
    };
}
