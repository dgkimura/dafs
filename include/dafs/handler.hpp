#pragma once

#include "dafs/metadataparser.hpp"
#include "dafs/node.hpp"
#include "dafs/sender.hpp"


namespace dafs
{
    dafs::Message
    HandleAllocateBlock(
        dafs::Node& node,
        dafs::Message message,
        dafs::Sender& sender);


    dafs::Message
    HandleReadBlock(
        dafs::Node& node,
        dafs::Message message,
        dafs::Sender& sender);


    dafs::Message
    HandleWriteBlock(
        dafs::Node& node,
        dafs::Message message,
        dafs::Sender& sender);


    dafs::Message
    HandleDeleteBlock(
        dafs::Node& node,
        dafs::Message message,
        dafs::Sender& sender);


    dafs::Message
    HandleGetNodeDetails(
        dafs::Node& node,
        dafs::Message message,
        dafs::Sender& sender);


    dafs::Message
    HandleJoinCluster(
        dafs::Node& node,
        dafs::Message message,
        dafs::Sender& sender);


    dafs::Message
    HandleRequestJoinCluster(
        dafs::Node& node,
        dafs::Message message,
        dafs::Sender& sender);


    dafs::Message
    HandleAcceptJoinCluster(
        dafs::Node& node,
        dafs::Message message,
        dafs::Sender& sender);


    dafs::Message
    HandleExitCluster(
        dafs::Node& node,
        dafs::Message message,
        dafs::Sender& sender);


    dafs::Message
    HandleProposeExitCluster(
        dafs::Node& node,
        dafs::Message message,
        dafs::Sender& sender);


    dafs::Message
    HandlePlusExitCluster(
        dafs::Node& node,
        dafs::Message message,
        dafs::Sender& sender);


    dafs::Message
    HandleMinusExitCluster(
        dafs::Node& node,
        dafs::Message message,
        dafs::Sender& sender);
}
