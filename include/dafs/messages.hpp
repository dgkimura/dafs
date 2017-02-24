#pragma once

#include <string>
#include <vector>

#include "dafs/metadata.hpp"


namespace dafs
{
    enum class MessageType
    {
        //
        // Read from a known block.
        //
        ReadBlock,

        //
        // Write to a known block.
        //
        WriteBlock,

        //
        // Deletes a known block.
        //
        DeleteBlock,

        //
        // Get details of the node.
        //
        GetNodeDetails,

        //
        // Join cluster - message sent to a node to signal it to join a
        // cluster.
        //
        _JoinCluster,

        //
        // Request initation - message sent to a node requesting to initiate
        // into minus partition on given cluster node.
        //
        _RequestMinusInitiation,

        //
        // Request initation - message sent to a node requesting to initiate
        // into plus partition on given cluster node.
        //
        _RequestPlusInitiation,

        //
        // Processes initiation - message sent to node in a cluster to signal
        // that an outside node is attempting to initiate itself into this
        // cluster.
        //
        _AcceptInitation,

        //
        // Conclude initation - message is sent to node to signal that the
        // initation process has finished and provide the result of the
        // initation request.
        //
        _ConcludeInitation,
    };


    struct Address
    {
        std::string ip;
        short port;

        Address()
        {
        }

        Address(std::string ip, short port)
            : ip(ip),
              port(port)
        {
        }
    };


    struct EmptyAddress : public Address
    {
        EmptyAddress()
            : Address("", 0)
        {
        }
    };


    struct Message
    {
        Address from;
        Address to;
        dafs::MessageType type;
        std::vector<dafs::MetaData> metadata;
    };


    struct EmptyMessage : public Message
    {
        EmptyAddress from;
        EmptyAddress to;
    };
}
