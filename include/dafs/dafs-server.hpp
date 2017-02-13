#pragma once

#include <string>

#include <boost/uuid/uuid.hpp>


const std::string IdentityFilename = "identity";


struct Options
{
    std::string settings_file;

    boost::uuids::uuid identity;

    std::string address;

    short port;

    short minus_port;

    short zero_port;

    short plus_port;
};
