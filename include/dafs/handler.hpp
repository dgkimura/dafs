#pragma once

#include "dafs/metadataparser.hpp"
#include "dafs/node.hpp"
#include "dafs/sender.hpp"


namespace dafs
{
    void
    HandleAllocateBlock(
        dafs::Node& node,
        dafs::Message message,
        std::shared_ptr<dafs::Sender> sender);


    void
    HandleReadBlock(
        dafs::Node& node,
        dafs::Message message,
        std::shared_ptr<dafs::Sender> sender);


    void
    HandleWriteBlock(
        dafs::Node& node,
        dafs::Message message,
        std::shared_ptr<dafs::Sender> sender);


    void
    HandleDeleteBlock(
        dafs::Node& node,
        dafs::Message message,
        std::shared_ptr<dafs::Sender> sender);


    void
    HandleGetNodeDetails(
        dafs::Node& node,
        dafs::Message message,
        std::shared_ptr<dafs::Sender> sender);


    void
    HandleJoinCluster(
        dafs::Node& node,
        dafs::Message message,
        std::shared_ptr<dafs::Sender> sender);


    void
    HandleRequestJoinCluster(
        dafs::Node& node,
        dafs::Message message,
        std::shared_ptr<dafs::Sender> sender);


    void
    HandleAcceptJoinCluster(
        dafs::Node& node,
        dafs::Message message,
        std::shared_ptr<dafs::Sender> sender);


    void
    HandleExitCluster(
        dafs::Node& node,
        dafs::Message message,
        std::shared_ptr<dafs::Sender> sender);


    void
    HandleProposeExitCluster(
        dafs::Node& node,
        dafs::Message message,
        std::shared_ptr<dafs::Sender> sender);


    void
    HandlePlusExitCluster(
        dafs::Node& node,
        dafs::Message message,
        std::shared_ptr<dafs::Sender> sender);


    void
    HandleMinusExitCluster(
        dafs::Node& node,
        dafs::Message message,
        std::shared_ptr<dafs::Sender> sender);
}
