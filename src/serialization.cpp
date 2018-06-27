#include <map>

#include <boost/lexical_cast.hpp>
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


    static dafs::Identity
    convert(const dafs::proto::Identity& obj)
    {
        dafs::Identity _obj;
        _obj.id = obj.id();
        return _obj;
    }


    static dafs::proto::BlockFormat
    convert(const dafs::BlockFormat& obj)
    {
        dafs::proto::BlockFormat _obj;
        _obj.set_contents(obj.contents);
        return _obj;
    }


    static dafs::BlockFormat
    convert(const dafs::proto::BlockFormat& obj)
    {
        dafs::BlockFormat _obj;
        _obj.contents = obj.contents();
        return _obj;
    }


    static dafs::proto::BlockInfo
    convert(const dafs::BlockInfo& obj)
    {
        dafs::proto::BlockInfo _obj;
        _obj.set_path(obj.path);
        *_obj.mutable_identity() = convert(obj.identity);
        _obj.set_revision(obj.revision);
        return _obj;
    }


    static dafs::BlockInfo
    convert(const dafs::proto::BlockInfo& obj)
    {
        dafs::Identity _obj_id = convert(obj.identity());

        dafs::BlockInfo _obj;
        _obj.path = obj.path();
        _obj.identity = _obj_id;
        _obj.revision = obj.revision();
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


    static dafs::Address
    convert(const dafs::proto::Address& obj)
    {
        dafs::Address _obj;
        _obj.ip = obj.ip();
        _obj.port = obj.port();
        return _obj;
    }


    static dafs::proto::Endpoint
    convert(const dafs::Endpoint& obj)
    {
        dafs::proto::Endpoint _obj;
        *_obj.mutable_management() = convert(obj.management);
        *_obj.mutable_replication() = convert(obj.replication);
        *_obj.mutable_identity() = convert(obj.identity);
        return _obj;
    }


    static dafs::Endpoint
    convert(const dafs::proto::Endpoint& obj)
    {
        dafs::Endpoint _obj;
        _obj.management = convert(obj.management());
        _obj.replication = convert(obj.replication());
        _obj.identity = convert(obj.identity());
        return _obj;
    }


    static dafs::proto::ReplicatedEndpoints
    convert(const dafs::ReplicatedEndpoints& obj)
    {
        dafs::proto::ReplicatedEndpoints _obj;
        *_obj.mutable_minus() = convert(obj.minus);
        *_obj.mutable_zero() = convert(obj.zero);
        *_obj.mutable_plus() = convert(obj.plus);
        return _obj;
    }


    static dafs::ReplicatedEndpoints
    convert(const dafs::proto::ReplicatedEndpoints& obj)
    {
        dafs::ReplicatedEndpoints _obj;
        _obj.minus = convert(obj.minus());
        _obj.zero = convert(obj.zero());
        _obj.plus = convert(obj.plus());
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


    static dafs::Proposal
    convert(const dafs::proto::Proposal& obj)
    {
        std::map<dafs::proto::Proposal::ProposalType, dafs::ProposalType> mtype_map =
        {
            { dafs::proto::Proposal::WRITE_BLOCK, dafs::ProposalType::WriteBlock  },
            { dafs::proto::Proposal::DELETE_BLOCK, dafs::ProposalType::DeleteBlock  },
            { dafs::proto::Proposal::PING, dafs::ProposalType::Ping  }
        };
        dafs::Proposal _obj;
        _obj.type = mtype_map[obj.type()];
        _obj.content = obj.content();
        _obj.uuid = boost::lexical_cast<boost::uuids::uuid>(obj.uuid());
        return _obj;
    }


    static dafs::proto::BlockIndex
    convert(const dafs::BlockIndex& obj)
    {
        dafs::proto::BlockIndex _obj;
        for (auto& block_info : obj.items)
        {
            auto *o = _obj.add_items();
            *o = convert(block_info);
        }
        return _obj;
    }


    static dafs::BlockIndex
    convert(const dafs::proto::BlockIndex& obj)
    {
        dafs::BlockIndex _obj;
        for (auto& block_info : obj.items())
        {
            _obj.items.push_back(convert(block_info));
        }
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


    static dafs::MetaData
    convert(dafs::proto::MetaData obj)
    {
        std::map<dafs::proto::MetaData::MetaDataType, std::string> mtype_map =
        {
            { dafs::proto::MetaData::BLOCK_INFO, "BlockInfo" },
            { dafs::proto::MetaData::BLOCK_FORMAT, "BlockFormat" },
            { dafs::proto::MetaData::ADDRESS, "Address" },
            { dafs::proto::MetaData::ENDPOINT, "Endpoint" },
            { dafs::proto::MetaData::IDENTIFIER, "Identity" },
            { dafs::proto::MetaData::NODE_ENDPOINTS, "NodeEndpoints" },
            { dafs::proto::MetaData::MINUS_REPLICATED_ENDPOINTS, "MinusReplicatedEndpoints" },
            { dafs::proto::MetaData::ZERO_REPLICATED_ENDPOINTS, "ZeroReplicatedEndpoints" },
            { dafs::proto::MetaData::PLUS_REPLICATED_ENDPOINTS, "PlusReplicatedEndpoints" },
            { dafs::proto::MetaData::MINUS_IDENTITY, "MinusIdentity" },
            { dafs::proto::MetaData::ZERO_IDENTITY, "ZeroIdentity" },
            { dafs::proto::MetaData::PLUS_IDENTITY, "PlusIdentity" }
        };
        dafs::MetaData _obj;
        _obj.key = mtype_map[obj.type()];
        _obj.value = obj.value();
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


    template <>
    std::string serialize(const dafs::Identity& obj)
    {
        std::string data;
        auto _obj = convert(obj);
        _obj.SerializeToString(&data);
        return data;
    }


    template <>
    dafs::Identity deserialize(std::string obj)
    {
        dafs::proto::Identity _obj;
        _obj.ParseFromString(obj);
        return convert(_obj);
    }


    template <>
    std::string serialize(const dafs::BlockFormat& obj)
    {
        std::string data;
        auto _obj = convert(obj);
        _obj.SerializeToString(&data);
        return data;
    }


    template <>
    dafs::BlockFormat deserialize(std::string obj)
    {
        dafs::proto::BlockFormat _obj;
        _obj.ParseFromString(obj);
        return convert(_obj);
    }


    template <>
    std::string serialize(const dafs::BlockInfo& obj)
    {
        std::string data;
        auto _obj = convert(obj);
        _obj.SerializeToString(&data);
        return data;
    }


    template <>
    dafs::BlockInfo deserialize(std::string obj)
    {
        dafs::proto::BlockInfo _obj;
        _obj.ParseFromString(obj);
        return convert(_obj);
    }


    template <>
    std::string serialize(const dafs::Address& obj)
    {
        std::string data;
        auto _obj = convert(obj);
        _obj.SerializeToString(&data);
        return data;
    }


    template <>
    dafs::Address deserialize(std::string obj)
    {
        dafs::proto::Address _obj;
        _obj.ParseFromString(obj);
        return convert(_obj);
    }


    template <>
    std::string serialize(const dafs::Endpoint& obj)
    {
        std::string data;
        auto _obj = convert(obj);
        _obj.SerializeToString(&data);
        return data;
    }


    template <>
    dafs::Endpoint deserialize(std::string obj)
    {
        dafs::proto::Endpoint _obj;
        _obj.ParseFromString(obj);
        return convert(_obj);
    }


    template <>
    std::string serialize(const dafs::ReplicatedEndpoints& obj)
    {
        std::string data;
        auto _obj = convert(obj);
        _obj.SerializeToString(&data);
        return data;
    }


    template <>
    dafs::ReplicatedEndpoints deserialize(std::string obj)
    {
        dafs::proto::ReplicatedEndpoints _obj;
        _obj.ParseFromString(obj);
        return convert(_obj);
    }


    template <>
    std::string serialize(const dafs::Proposal& obj)
    {
        std::string data;
        auto _obj = convert(obj);
        _obj.SerializeToString(&data);
        return data;
    }


    template <>
    dafs::Proposal deserialize(std::string obj)
    {
        dafs::proto::Proposal _obj;
        _obj.ParseFromString(obj);
        return convert(_obj);
    }


    template <>
    std::string serialize(const dafs::BlockIndex& obj)
    {
        std::string data;
        auto _obj = convert(obj);
        _obj.SerializeToString(&data);
        return data;
    }


    template <>
    dafs::BlockIndex deserialize(std::string obj)
    {
        dafs::proto::BlockIndex _obj;
        _obj.ParseFromString(obj);
        return convert(_obj);
    }


    template <>
    std::string serialize(const dafs::MetaData& obj)
    {
        std::string data;
        auto _obj = convert(obj);
        _obj.SerializeToString(&data);
        return data;
    }


    template <>
    dafs::MetaData deserialize(std::string obj)
    {
        dafs::proto::MetaData _obj;
        _obj.ParseFromString(obj);
        return convert(_obj);
    }
}
