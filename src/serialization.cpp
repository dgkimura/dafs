#include "message.pb.h"

#include "dafs/serialization.hpp"


namespace dafs
{
    std::string serialize(const dafs::Address& address)
    {
        std::string data;
        dafs::proto::Address _address;
        _address.set_ip(address.ip);
        _address.set_port(address.port);
        _address.SerializeToString(&data);
        return data;
    }
}
