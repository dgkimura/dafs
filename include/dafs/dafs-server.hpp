#pragma once

#include <string>

#include <boost/uuid/uuid.hpp>


struct Options
{
    std::string settings_file;

    boost::uuids::uuid identity;

    std::string address;

    short port;

    short minus_port;

    short zero_port;

    short plus_port;

    long long int minus_ping_interval;

    long long int zero_ping_interval;

    long long int plus_ping_interval;
};
