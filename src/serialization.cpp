#include <map>

#include <boost/uuid/uuid.hpp>

#include "message.pb.h"
#include "dafs/serialization.hpp"


namespace dafs
{
    static dafs::proto::Delta
    convert(const dafs::Delta& obj)
    {
        dafs::proto::Delta _obj;
        _obj.set_difference(obj.difference);
        return _obj;
    }


    static dafs::Delta
    convert(const dafs::proto::Delta& obj)
    {
        dafs::Delta _obj;
        _obj.difference = obj.difference();
        return _obj;
    }


    static dafs::proto::Identity
    convert(const dafs::Identity& obj)
    {
        dafs::proto::Identity _obj;
        _obj.set_id(obj.id);
        return _obj;
    }


    static dafs::proto::BlockFormat
    convert(const dafs::BlockFormat& obj)
    {
        dafs::proto::BlockFormat _obj;
        _obj.set_contents(obj.contents);
        return _obj;
    }


    static dafs::proto::BlockInfo
    convert(const dafs::BlockInfo& obj)
    {
        dafs::proto::Identity _obj_id = convert(obj.identity);

        dafs::proto::BlockInfo _obj;
        _obj.set_path(obj.path);
        _obj.set_allocated_identity(&_obj_id);
        _obj.set_revision(obj.revision);
        return _obj;
    }


    static dafs::proto::Address
    convert(const dafs::Address& obj)
    {
        dafs::proto::Address _obj;
        _obj.set_ip(obj.ip);
        _obj.set_port(obj.port);
        return _obj;
    }


    static dafs::proto::Endpoint
    convert(const dafs::Endpoint& obj)
    {
        dafs::proto::Identity _obj_id = convert(obj.identity);
        dafs::proto::Address _obj_management = convert(obj.management);
        dafs::proto::Address _obj_replication = convert(obj.replication);

        dafs::proto::Endpoint _obj;
        _obj.set_allocated_management(&_obj_management);
        _obj.set_allocated_replication(&_obj_replication);
        _obj.set_allocated_identity(&_obj_id);
        return _obj;
    }


    static dafs::proto::ReplicatedEndpoints
    convert(const dafs::ReplicatedEndpoints& obj)
    {
        dafs::proto::Endpoint _obj_minus = convert(obj.minus);
        dafs::proto::Endpoint _obj_zero = convert(obj.zero);
        dafs::proto::Endpoint _obj_plus = convert(obj.plus);

        dafs::proto::ReplicatedEndpoints _obj;
        _obj.set_allocated_minus(&_obj_minus);
        _obj.set_allocated_zero(&_obj_zero);
        _obj.set_allocated_plus(&_obj_plus);
        return _obj;
    }


    static dafs::proto::Proposal
    convert(const dafs::Proposal& obj)
    {
        std::map<dafs::ProposalType, dafs::proto::Proposal::ProposalType> mtype_map =
        {
            { dafs::ProposalType::WriteBlock, dafs::proto::Proposal::WRITE_BLOCK },
            { dafs::ProposalType::DeleteBlock, dafs::proto::Proposal::DELETE_BLOCK },
            { dafs::ProposalType::Ping, dafs::proto::Proposal::PING }
        };
        dafs::proto::Proposal _obj;
        _obj.set_type(mtype_map[obj.type]);
        _obj.set_content(obj.content);
        _obj.set_uuid(boost::uuids::to_string(obj.uuid));
        return _obj;
    }


    static dafs::proto::MetaData
    convert(dafs::MetaData obj)
    {
        std::map<std::string, dafs::proto::MetaData::MetaDataType> mtype_map =
        {
            { "BlockInfo", dafs::proto::MetaData::BLOCK_INFO },
            { "BlockFormat", dafs::proto::MetaData::BLOCK_FORMAT },
            { "Address", dafs::proto::MetaData::ADDRESS },
            { "Endpoint", dafs::proto::MetaData::ENDPOINT },
            { "Identity", dafs::proto::MetaData::IDENTIFIER },
            { "NodeEndpoints", dafs::proto::MetaData::NODE_ENDPOINTS },
            { "MinusReplicatedEndpoints", dafs::proto::MetaData::MINUS_REPLICATED_ENDPOINTS },
            { "ZeroReplicatedEndpoints", dafs::proto::MetaData::ZERO_REPLICATED_ENDPOINTS },
            { "PlusReplicatedEndpoints", dafs::proto::MetaData::PLUS_REPLICATED_ENDPOINTS },
            { "MinusIdentity", dafs::proto::MetaData::MINUS_IDENTITY },
            { "ZeroIdentity", dafs::proto::MetaData::ZERO_IDENTITY },
            { "PlusIdentity", dafs::proto::MetaData::PLUS_IDENTITY }
        };
        dafs::proto::MetaData _obj;
        _obj.set_type(mtype_map[obj.key]);
        _obj.set_value(obj.value);
        return _obj;
    }


    static dafs::proto::Message
    convert(dafs::Message obj)
    {
        dafs::proto::Address _obj_from = convert(obj.from);
        dafs::proto::Address _obj_to = convert(obj.to);
        std::map<dafs::MessageType, dafs::proto::Message::MessageType> mtype_map =
        {
            { dafs::MessageType::Success, dafs::proto::Message::SUCCESS },
            { dafs::MessageType::Failure, dafs::proto::Message::FAILURE },
            { dafs::MessageType::AllocateBlock, dafs::proto::Message::ALLOCATE_BLOCK },
            { dafs::MessageType::ReadBlock, dafs::proto::Message::READ_BLOCK },
            { dafs::MessageType::WriteBlock, dafs::proto::Message::WRITE_BLOCK },
            { dafs::MessageType::DeleteBlock, dafs::proto::Message::DELETE_BLOCK },
            { dafs::MessageType::GetNodeDetails, dafs::proto::Message::GET_NODE_DETAILS },
            { dafs::MessageType::_JoinCluster, dafs::proto::Message::JOIN_CLUSTER },
            { dafs::MessageType::_RequestJoinCluster, dafs::proto::Message::REQUEST_JOIN_CLUSTER },
            { dafs::MessageType::_AcceptJoinCluster, dafs::proto::Message::ACCEPT_JOIN_CLUSTER },
            { dafs::MessageType::ExitCluster, dafs::proto::Message::EXIT_CLUSTER },
            { dafs::MessageType::_ProposeExitCluster, dafs::proto::Message::PROPOSE_EXIT_CLUSTER },
            { dafs::MessageType::_PlusExitCluster, dafs::proto::Message::PLUS_EXIT_CLUSTER },
            { dafs::MessageType::_MinusExitCluster, dafs::proto::Message::MINUS_EXIT_CLUSTER }
        };

        dafs::proto::Message _obj;
        _obj.set_allocated_from_address(&_obj_from);
        _obj.set_allocated_to_address(&_obj_to);
        _obj.set_type(mtype_map[obj.type]);

        for (auto& md : obj.metadata)
        {
            auto *o = _obj.add_metadata();
            *o = convert(md);
        }
        return _obj;
    }


    template <>
    std::string serialize(const dafs::Delta& obj)
    {
        std::string data;
        auto _obj = convert(obj);
        _obj.SerializeToString(&data);
        return data;
    }


    template <>
    dafs::Delta deserialize(std::string obj)
    {
        dafs::proto::Delta _obj;
        _obj.ParseFromString(obj);
        return convert(_obj);
    }
}
