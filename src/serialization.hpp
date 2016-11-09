#pragma once

#include <fstream>
#include <sstream>

#include "boost/archive/text_iarchive.hpp"
#include "boost/archive/text_oarchive.hpp"

#include "delta.hpp"
#include "filesystem.hpp"
#include "messages.hpp"
#include "proposals.hpp"


namespace dafs
{
    template <typename Archive>
    void serialize(Archive& ar, dafs::Delta& obj, const unsigned int version)
    {
        ar & obj.filename;
        ar & obj.difference;
    }


    template <typename Archive>
    void serialize(Archive& ar, dafs::Bytes& obj, const unsigned int version)
    {
        ar & obj.contents;
    }


    template <typename Archive>
    void serialize(Archive& ar, dafs::Location& obj, const unsigned int version)
    {
        ar & obj.address;
    }


    template <typename Archive>
    void serialize(Archive& ar, dafs::BlockFormat& obj, const unsigned int version)
    {
        ar & obj.contents;
    }


    template <typename Archive>
    void serialize(Archive& ar, dafs::BlockInfo& obj, const unsigned int version)
    {
        ar & obj.filename;
        ar & obj.location;
        ar & obj.offset;
        ar & obj.revision;
    }


    template <typename Archive>
    void serialize(Archive& ar, dafs::FileFormat& obj, const unsigned int version)
    {
        ar & obj.info;
        for (int i=0; i<BLOCKS_IN_FILEINFO; i++)
        {
            ar & obj.blocks[i];
        }
    }


    template <typename Archive>
    void serialize(Archive& ar, dafs::FileInfo& obj, const unsigned int version)
    {
        ar & obj.previous;
        ar & obj.next;
        ar & obj.descriptor;
        ar & obj.name;
    }


    template <typename Archive>
    void serialize(Archive& ar, dafs::FileIndex& obj, const unsigned int version)
    {
        int size;

        if (Archive::is_loading::value)
        {
            //
            // deserialize collection object.
            //
            ar & size;
            for (int i=0; i<size; i++)
            {
                FileInfo info;
                ar & info;
                obj.files.push_back(info);
            }
        }
        else
        {
            //
            // serialize collection object.
            //
            size = obj.files.size();
            ar & size;
            for (int i=0; i<size; i++)
            {
                ar & obj.files[i];
            }
        }
    }


    template <typename Archive>
    void serialize(Archive& ar, dafs::BlockIndex& obj, const unsigned int version)
    {
        int size;

        if (Archive::is_loading::value)
        {
            //
            // deserialize collection object.
            //
            ar & size;
            for (int i=0; i<size; i++)
            {
                BlockInfo info;
                ar & info;
                obj.blocks.push_back(info);
            }
        }
        else
        {
            //
            // serialize collection object.
            //
            size = obj.blocks.size();
            ar & size;
            for (int i=0; i<size; i++)
            {
                ar & obj.blocks[i];
            }
        }
    }


    template <typename Archive>
    void serialize(Archive& ar, dafs::MetaData& obj, const unsigned int version)
    {
        ar & obj.key;
        ar & obj.value;
    }


    template <typename Archive>
    void serialize(Archive& ar, dafs::Message& obj, const unsigned int version)
    {
        ar & obj.from;
        ar & obj.to;
        ar & obj.content;
        ar & obj.type;

        int size;

        if (Archive::is_loading::value)
        {
            //
            // deserialize collection object.
            //
            ar & size;
            for (int i=0; i<size; i++)
            {
                MetaData md;
                ar & md;
                obj.metadata.push_back(md);
            }
        }
        else
        {
            //
            // serialize collection object.
            //
            size = obj.metadata.size();
            ar & size;
            for (int i=0; i<size; i++)
            {
                ar & obj.metadata[i];
            }
        }
    }


    template <typename Archive>
    void serialize(Archive& ar, dafs::Proposal& obj, const unsigned int version)
    {
        ar & obj.type;
        ar & obj.content;
    }


    template <typename Archive>
    void serialize(Archive& ar, dafs::BlockEdit& obj, const unsigned int version)
    {
        ar & obj.info;
        ar & obj.change;
        ar & obj.hash;
        ar & obj.revision;
    }


    template <typename T>
    std::string Serialize(T object)
    {
        std::stringstream stream;
        boost::archive::text_oarchive oa(stream);
        oa << object;
        return stream.str();
    }


    template <typename T>
    T Deserialize(std::string string_obj)
    {
        T object;
        std::stringstream stream(string_obj);
        boost::archive::text_iarchive oa(stream);
        oa >> object;
        return object;
    }


    template <typename T>
    T Deserialize(std::fstream& stream)
    {
        T object;
        boost::archive::text_iarchive oa(stream);
        oa >> object;
        return object;
    }
}
