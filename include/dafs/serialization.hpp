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
    void serialize(Archive& ar, dafs::Identity& obj, const unsigned int version)
    {
        ar & obj.id;
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
        ar & obj.fault_domain;
    }


    template <typename Archive>
    void serialize(Archive& ar, dafs::ReplicatedEndpoints& obj, const unsigned int version)
    {
        ar & obj.minus;
        ar & obj.zero;
        ar & obj.plus;
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
