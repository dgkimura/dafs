#pragma once

#include <map>
#include <limits>
#include <sstream>

#include <boost/filesystem.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <string>
#include <vector>


namespace dafs
{
    const int BLOCK_SIZE_IN_BYTES = 65536;

    const int BLOCKS_IN_FILEINFO = 8192;

    //
    // Defines directory where partition is located.
    //
    struct Root
    {
        std::string directory;

        Root(std::string directory)
            : directory(directory)
        {
        }
    };


    //
    // Defines segment of bytes. (used to write to blocks)
    //
    struct Bytes
    {
        char contents[BLOCK_SIZE_IN_BYTES];
    };


    //
    // Associates stored objects with partitions.
    //
    struct Identity
    {
        boost::uuids::string_generator gen;

        std::string id;

        Identity()
        {
            id = "00000000-0000-0000-0000-000000000000";
        }

        Identity(std::string uuid)
        {
            id = uuid;
        }

        bool
        operator==(const Identity& rhs) const
        {
            return gen(id) == gen(rhs.id);
        }

        bool
        operator!=(const Identity& rhs) const
        {
            return gen(id) != gen(rhs.id);
        }

        bool
        operator<(const Identity& rhs) const
        {
            return gen(id) < gen(rhs.id);
        }

        bool
        operator>(const Identity& rhs) const
        {
            return gen(id) > gen(rhs.id);
        }

        bool
        operator<=(const Identity& rhs) const
        {
            return gen(id) <= gen(rhs.id);
        }

        bool
        operator>=(const Identity& rhs) const
        {
            return gen(id) >= gen(rhs.id);
        }

        Identity
        operator+(const Identity& rhs) const
        {
            std::string raw_result;

            bool carry = false;
            for (int i = id.size() - 1; i >= 0; i--)
            {
                if (id[i] == '-')
                {
                    raw_result = '-' + raw_result;
                    continue;
                }
                int a;
                int b;
                int r;
                std::stringstream(std::string(1, id[i])) >> std::hex >> a;
                std::stringstream(std::string(1, rhs.id[i])) >> std::hex >> b;
                r = a + b + (carry ? 1 : 0);

                std::stringstream tmp;
                tmp << std::hex << (r);
                raw_result = tmp.str()[tmp.str().size() - 1] + raw_result;

                carry = r > 0xF ? true : false;
            }
            dafs::Identity result(raw_result);
            return result;
        }

        Identity
        operator-(const Identity& rhs) const
        {
            std::string raw_result;

            bool borrow = false;
            for (int i = id.size() - 1; i >= 0; i--)
            {
                if (id[i] == '-')
                {
                    raw_result = '-' + raw_result;
                    continue;
                }
                int a;
                int b;
                std::stringstream(std::string(1, id[i])) >> std::hex >> a;
                std::stringstream(std::string(1, rhs.id[i])) >> std::hex >> b;
                if (borrow)
                {
                    a -= 1;
                }
                if (a < b)
                {
                    borrow = true;
                    a += 0xF + 1;
                }
                else
                {
                    borrow = false;
                }
                std::stringstream tmp;
                tmp << std::hex << (a - b);
                raw_result = tmp.str() + raw_result;
            }
            dafs::Identity result(raw_result);
            return result;
        }
    };


    //
    // Describes the block info.
    //
    struct BlockInfo
    {
        std::string path;

        Identity identity;

        int revision;

        bool
        operator==(const BlockInfo& rhs) const
        {
            return path == rhs.path && revision == rhs.revision;
        }
    };


    //
    // Defines the block structure on disk.
    //
    struct BlockFormat
    {
        std::string contents;
    };


    //
    // Describes the file info.
    //
    struct FileInfo
    {
        Identity current;

        Identity previous;

        Identity next;

        std::string path;

        bool
        operator==(const FileInfo& rhs) const
        {
            return path == rhs.path && current == rhs.current;
        }
    };


    //
    // Defines the file structure on disk.
    //
    struct FileFormat
    {
        FileInfo info;

        BlockInfo blocks[BLOCKS_IN_FILEINFO];
    };


    template<typename T>
    struct Index
    {
        std::vector<T> items;
    };


    using FileIndex = Index<FileInfo>;


    using BlockIndex = Index<BlockInfo>;


    //
    // Function to create Bytes
    //
    Bytes CreateBytes(const std::string& data);


    bool IsLogicallyOrdered(dafs::Identity first,
                            dafs::Identity second,
                            dafs::Identity third);


    BlockIndex SplitUpperIndex(BlockIndex index,
                               dafs::Identity lower,
                               dafs::Identity divider,
                               dafs::Identity upper);


    BlockIndex SplitLowerIndex(BlockIndex index,
                               dafs::Identity lower,
                               dafs::Identity divider,
                               dafs::Identity upper);
}
