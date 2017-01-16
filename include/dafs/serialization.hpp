#pragma once

#include <fstream>
#include <sstream>

#include "boost/archive/text_iarchive.hpp"
#include "boost/archive/text_oarchive.hpp"

#include "delta.hpp"
#include "filesystem.hpp"
#include "messages.hpp"
#include "propose.hpp"


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
        ar & obj.port;
    }


    template <typename Archive>
    void serialize(Archive& ar, dafs::BlockFormat& obj, const unsigned int version)
    {
        ar & obj.contents;
    }


    template <typename Archive>
    void serialize(Archive& ar, dafs::BlockInfo& obj, const unsigned int version)
    {
        ar & obj.path;
        ar & obj.location;
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
        ar & obj.path;
    }


    template <typename Archive, typename T>
    void serialize(Archive& ar, dafs::Index<T>& obj, const unsigned int version)
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
                T info;
                ar & info;
                obj.items.push_back(info);
            }
        }
        else
        {
            //
            // serialize collection object.
            //
            size = obj.items.size();
            ar & size;
            for (int i=0; i<size; i++)
            {
                ar & obj.items[i];
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
    void serialize(Archive& ar, dafs::ProposalContent& obj, const unsigned int version)
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
    std::string SerializeIntoBlockFormat(T object)
    {
        dafs::BlockFormat b;
        b.contents = Serialize(object);
        return Serialize(b);
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


    template <typename T>
    T DeserializeFromBlockFormat(std::fstream& stream)
    {
        dafs::BlockFormat b = Deserialize<dafs::BlockFormat>(stream);
        return Deserialize<T>(b.contents);
    }
}