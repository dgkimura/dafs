#pragma once

#include "dafs/metadataparser.hpp"
#include "dafs/node.hpp"
#include "dafs/sender.hpp"


namespace dafs
{
    dafs::Message
    HandleReadBlock(
        dafs::Node& node,
        dafs::MetaDataParser metadata);


    dafs::Message
    HandleWriteBlock(
        dafs::Node& node,
        dafs::MetaDataParser metadata);


    dafs::Message
    HandleDeleteBlock(
        dafs::Node& node,
        dafs::MetaDataParser metadata);


    dafs::Message
    HandleGetNodeDetails(
        dafs::Node& node,
        dafs::MetaDataParser metadata);


    dafs::Message
    HandleJoinCluster(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender);


    dafs::Message
    HandleRequestMinusInitiation(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender);


    dafs::Message
    HandleRequestPlusInitiation(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender);


    dafs::Message
    HandleAcceptPlusInitiation(
        dafs::Node& node,
        dafs::MetaDataParser metadata);


    dafs::Message
    HandleExitCluster(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender);


    dafs::Message
    HandleProposeExitCluster(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender);


    dafs::Message
    HandlePlusExitCluster(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender);


    dafs::Message
    HandleMinusExitCluster(
        dafs::Node& node,
        dafs::MetaDataParser metadata,
        dafs::Sender& sender);
}
