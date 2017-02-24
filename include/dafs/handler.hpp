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
        dafs::MetaDataParser metadata);


    dafs::Message
    HandleRequestMinusInitiation(
        dafs::Node& node,
        dafs::MetaDataParser metadata);


    dafs::Message
    HandleRequestPlusInitiation(
        dafs::Node& node,
        dafs::MetaDataParser metadata);


    dafs::Message
    HandleAcceptInitiation(
        dafs::Node& node,
        dafs::MetaDataParser metadata);


    dafs::Message
    HandleConcludeInitation(
        dafs::Node& node,
        dafs::MetaDataParser metadata);
}
