#pragma once

#include <cstdint>
#include <string>

#include <boost/uuid/uuid.hpp>


struct Options
{
    std::string settings_file;

    boost::uuids::uuid identity;

    std::string address;

    uint16_t port;

    uint16_t minus_port;

    uint16_t zero_port;

    uint16_t plus_port;

    int64_t minus_ping_interval;

    int64_t zero_ping_interval;

    int64_t plus_ping_interval;
};
