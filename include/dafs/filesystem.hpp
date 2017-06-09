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

        Identity&
        operator+=(int value)
        {
            boost::uuids::uuid raw_id = gen(id);

            unsigned char uuid_data[raw_id.size()];
            std::memcpy(&uuid_data, &raw_id, raw_id.size());

            unsigned char carry = 0;
            for (int i=raw_id.size()-1; i>0; i--)
            {
                auto max = std::numeric_limits<unsigned char>::max();
                int updated = uuid_data[i] + carry + (max & value);
                uuid_data[i] = updated;
                carry = (updated > max ? 1 : 0);
                value = value >> std::numeric_limits<unsigned char>::digits;
            }

            boost::uuids::uuid u;
            std::memcpy(&u, uuid_data, raw_id.size());
            id = boost::uuids::to_string(u);

            return *this;
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
