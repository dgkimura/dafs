#pragma once

#include <string>
#include <vector>


namespace dafs
{
    struct MetaData
    {
        std::string key;
        std::string value;
    };


    const std::string BlockInfoKey = "BlockInfo";


    const std::string BlockFormatKey = "BlockFormat";


    const std::string AddressKey = "Address";


    const std::string EndpointKey = "Endpoint";


    const std::string IdentityKey = "Identity";


    const std::string NodeEndpointsKey = "NodeEndpoints";


    const std::string MinusReplicatedEndpointsKey = "MinusReplicatedEndpoints";


    const std::string ZeroReplicatedEndpointsKey = "ZeroReplicatedEndpoints";


    const std::string PlusReplicatedEndpointsKey = "PlusReplicatedEndpoints";


    const std::string MinusIdentityKey = "MinusIdentity";


    const std::string ZeroIdentityKey = "ZeroIdentity";


    const std::string PlusIdentityKey = "PlusIdentity";
}
