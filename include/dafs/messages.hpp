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
        _RequestJoinCluster,

        //
        // Accepted inivitation - message sent to node to indicate that it
        // was accepted as the plus partition.
        //
        _AcceptJoinCluster,

        //
        // Exit cluster - message sent to a node to signal it to exit a
        // cluster.
        //
        ExitCluster,

        //
        // Propose exit cluster - message sent to either replicated partition
        // node suggesting to remove a non-responsive node.
        //
        _ProposeExitCluster,

        //
        // Plus exit cluster - message sent to node requesting to remove the
        // plus partition.
        //
        _PlusExitCluster,

        //
        // Minus exit cluster - message sent to node requesting to remove the
        // minus partition.
        //
        _MinusExitCluster
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


    static constexpr int MessageHeaderSize = 12;


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
