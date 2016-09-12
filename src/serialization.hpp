#ifndef __DAFS_SERIALIZATION_HPP_INCLUDED__
#define __DAFS_SERIALIZATION_HPP_INCLUDED__

#include <fstream>
#include <sstream>

#include "boost/archive/text_iarchive.hpp"
#include "boost/archive/text_oarchive.hpp"

#include "messages.hpp"


namespace dafs
{
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
        for (auto m : obj.metadata)
        {
            ar & m.key;
            ar & m.value;
        }
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
}


#endif
