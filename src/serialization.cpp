#include "message.pb.h"

#include "dafs/serialization.hpp"


namespace dafs
{
    std::string serialize(const dafs::Delta& obj)
    {
        std::string data;
        dafs::proto::Delta _obj;
        _obj.set_difference(obj.difference);
        _obj.SerializeToString(&data);
        return data;
    }


    std::string serialize(const dafs::Identity& obj)
    {
        std::string data;
        dafs::proto::Identity _obj;
        _obj.set_id(obj.id);
        _obj.SerializeToString(&data);
        return data;
    }


    std::string serialize(const dafs::BlockFormat& obj)
    {
        std::string data;
        dafs::proto::BlockFormat _obj;
        _obj.set_contents(obj.contents);
        _obj.SerializeToString(&data);
        return data;
    }


    std::string serialize(const dafs::BlockInfo& obj)
    {
        std::string data;

        dafs::proto::Identity _obj_id;
        _obj_id.set_id(obj.identity.id);

        dafs::proto::BlockInfo _obj;
        _obj.set_path(obj.path);
        _obj.set_allocated_identity(&_obj_id);
        _obj.set_revision(obj.revision);
        _obj.SerializeToString(&data);
        return data;
    }


    std::string serialize(const dafs::Address& obj)
    {
        std::string data;
        dafs::proto::Address _obj;
        _obj.set_ip(obj.ip);
        _obj.set_port(obj.port);
        _obj.SerializeToString(&data);
        return data;
    }
}
