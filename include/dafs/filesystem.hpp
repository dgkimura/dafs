#pragma once

#include <map>
#include <limits>

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
        boost::uuids::uuid id;

        Identity()
        {
            id = boost::uuids::nil_uuid();
        }

        Identity(std::string uuid)
        {
            boost::uuids::string_generator gen;
            id = gen(uuid);
        }

        bool
        operator==(const Identity& rhs) const
        {
            return id == rhs.id;
        }

        bool
        operator!=(const Identity& rhs) const
        {
            return id != rhs.id;
        }

        bool
        operator<(const Identity& rhs) const
        {
            return id < rhs.id;
        }

        bool
        operator>(const Identity& rhs) const
        {
            return id > rhs.id;
        }

        bool
        operator<=(const Identity& rhs) const
        {
            return id <= rhs.id;
        }

        bool
        operator>=(const Identity& rhs) const
        {
            return id >= rhs.id;
        }

        Identity
        operator+(const Identity& rhs) const
        {
            unsigned char rhs_uuid_data[16];
            boost::uuids::uuid rhs_id = rhs.id;
            std::memcpy(rhs_uuid_data, &rhs_id, 16);

            unsigned char lhs_uuid_data[16];
            boost::uuids::uuid lhs_id = id;
            std::memcpy(lhs_uuid_data, &lhs_id, 16);

            unsigned char result_uuid_data[16];
            bool carry = false;
            for (int i = 15; i >= 0; i--)
            {
                short result_data = lhs_uuid_data[i] + rhs_uuid_data[i];
                result_uuid_data[i] = result_data + (carry ? 1 : 0);
                carry = result_data > std::numeric_limits<unsigned char>::max() ?
                        true : false;
            }
            dafs::Identity result;
            std::memcpy(&result, result_uuid_data, 16);
            return result;
        }

        Identity
        operator-(const Identity& rhs) const
        {
            unsigned char rhs_uuid_data[16];
            boost::uuids::uuid rhs_id = rhs.id;
            std::memcpy(rhs_uuid_data, &rhs_id, 16);

            unsigned char lhs_uuid_data[16];
            boost::uuids::uuid lhs_id = id;
            std::memcpy(lhs_uuid_data, &lhs_id, 16);

            unsigned char result_uuid_data[16];
            bool borrow = false;
            for (int i = 15; i >= 0; i--)
            {
                unsigned short a = lhs_uuid_data[i];
                unsigned short b = rhs_uuid_data[i];
                if (borrow)
                {
                    a -= 1;
                }
                if (a < b)
                {
                    borrow = true;
                    a += std::numeric_limits<unsigned char>::max() + 1;
                }
                else
                {
                    borrow = false;
                }
                result_uuid_data[i] = a - b;
            }
            dafs::Identity result;
            std::memcpy(&result, result_uuid_data, 16);
            return result;
        }

        Identity
        Median(const Identity& rhs) const
        {
            Identity upper = rhs > *this ? rhs : *this;
            Identity lower = rhs > *this ? *this : rhs;

            Identity diff = upper - lower;

            unsigned char result_uuid_data[16];
            std::memcpy(result_uuid_data, &diff, 16);

            for (int i = 15; i >= 0; i--)
            {
                result_uuid_data[i] = (result_uuid_data[i] >> 1);
            }
            dafs::Identity result;
            std::memcpy(&result, result_uuid_data, 16);
            dafs::Identity median = result + lower;
            return median;
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


    //
    // Function to create BlockInfo
    //
    BlockInfo CreateBlockInfo(const std::string& path,
                              const dafs::Identity identity);
}
