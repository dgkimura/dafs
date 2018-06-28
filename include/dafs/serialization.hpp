#pragma once

#include <fstream>
#include <sstream>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/uuid/uuid_serialize.hpp>

#include "blocks.hpp"
#include "delta.hpp"
#include "details.hpp"
#include "identity.hpp"
#include "messages.hpp"
#include "propose.hpp"


namespace dafs
{
    template <typename Archive>
    void serialize(Archive& ar, dafs::Delta& obj, const unsigned int version)
    {
        ar & obj.difference;
    }


    template <typename Archive>
    void serialize(Archive& ar, dafs::Identity& obj, const unsigned int version)
    {
        ar & obj.id;
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
        ar & obj.identity;
        ar & obj.revision;
    }


    template <typename Archive, typename T>
    void serialize(Archive& ar, dafs::Index<T>& obj, const unsigned int version)
    {
        ar & obj.items;
    }


    template <typename Archive>
    void serialize(Archive& ar, dafs::Address& obj, const unsigned int version)
    {
        ar & obj.ip;
        ar & obj.port;
    }


    template <typename Archive>
    void serialize(Archive& ar, dafs::Endpoint& obj, const unsigned int version)
    {
        ar & obj.management;
        ar & obj.replication;
        ar & obj.identity;
    }


    template <typename Archive>
    void serialize(Archive& ar, dafs::ReplicatedEndpoints& obj, const unsigned int version)
    {
        ar & obj.minus;
        ar & obj.zero;
        ar & obj.plus;
    }


    template <typename Archive>
    void serialize(Archive& ar, dafs::Proposal& obj, const unsigned int version)
    {
        ar & obj.type;
        ar & obj.content;
        ar & obj.uuid;
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
    T Deserialize(std::string string_obj)
    {
        T object;
        std::stringstream stream(string_obj);
        try
        {
            boost::archive::text_iarchive oa(stream);
            oa >> object;
        }
        catch (boost::archive::archive_exception& e)
        {
        }
        return object;
    }


    template <typename T>
    T Deserialize(std::istream& stream)
    {
        T object;
        try
        {
            boost::archive::text_iarchive oa(stream);
            oa >> object;
        }
        catch (boost::archive::archive_exception& e)
        {
        }
        return object;
    }


    template <typename T>
    std::string serialize(const T& obj);


    template <typename T>
    T deserialize(std::string obj);
}
